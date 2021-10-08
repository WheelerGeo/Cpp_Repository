#include "../include/Logger.h"

Logger::Logger(void):level_(FNLog::PRIORITY_ERROR), device_id_(0) {
    loggerInit();
};

int Logger::loggerInit(void) {
    FNLog::Logger& logger = FNLog::GetDefaultLogger();
    FNLog::Logger::StateLockGuard state_guard(logger.state_lock);
    if (logger.logger_state_ != FNLog::LOGGER_STATE_UNINIT) {
        perror("init logger failed");
        return -1;
    }
    if (logger.shm_ == nullptr) {
        perror("init logger error. no shm");
        return -2;
    }

    logger.yaml_path_ = "";
    logger.hot_update_ = false;
    logger.shm_->channel_size_ = 1;
    memset(&logger.shm_->channels_, 0, sizeof(logger.shm_->channels_));

    FNLog::Channel& channel = logger.shm_->channels_[0];
    memset(&channel, 0, sizeof(channel));
    channel.channel_id_ = 0;
    channel.channel_type_ = FNLog::CHANNEL_ASYNC;
    channel.config_fields_[FNLog::CHANNEL_CFG_PRIORITY] = FNLog::PRIORITY_TRACE;
    channel.config_fields_[FNLog::CHANNEL_CFG_CATEGORY] = 0;
    channel.config_fields_[FNLog::CHANNEL_CFG_CATEGORY_EXTEND] = 0;

    if (logger.shm_->channel_size_ > FNLog::Logger::MAX_CHANNEL_SIZE
            || logger.shm_->channel_size_ <= 0) {
        printf("start error 2");
        return -2;
    }

    return 0;
}

int Logger::loggerStart(void) {
    int ret = -1;

    if (!config_path_.empty()) {
        ret = FNLog::LoadAndStartDefaultLogger(config_path_);
        LogAlarm() << "Log init success.";
        return ret;
    }

    ret = FNLog::StartLogger(FNLog::GetDefaultLogger());

    LogInfo() << "Log init success.";

    return ret;
}

Logger& Logger::getInstance(void) {
    static Logger instance;
    return instance;
}

void Logger::setLoggerFromConfig(const std::string &config_path) {
    if(!config_path_.empty()) {
        config_path_ = config_path;
    }
}

int Logger::setLoggerSync(void) {
    FNLog::Logger& logger = FNLog::GetDefaultLogger();
    FNLog::Logger::StateLockGuard state_guard(logger.state_lock);
    if (logger.logger_state_ != FNLog::LOGGER_STATE_UNINIT) {
        perror("init logger failed");
        return -1;
    }
    if (logger.shm_ == nullptr) {
        perror("init logger error. no shm");
        return -2;
    }

    logger.shm_->channels_[0].channel_type_ = FNLog::CHANNEL_SYNC;
    return 0;
}

int Logger::addLoggerToFile(const std::string& fpath, const std::string& fname,
            FNLog::LogPriority level, int limit_size, int rollback) {
    FNLog::Logger& logger = FNLog::GetDefaultLogger();
    FNLog::Logger::StateLockGuard state_guard(logger.state_lock);
    if (logger.logger_state_ != FNLog::LOGGER_STATE_UNINIT) {
        perror("init logger failed");
        return -1;
    }
    if (logger.shm_ == nullptr) {
        perror("init logger error. no shm");
        return -2;
    }

    FNLog::Channel& channel = logger.shm_->channels_[0];
    FNLog::Device& device = channel.devices_[channel.device_size_++];
    memset(&device, 0, sizeof(device));
    device.device_id_ = device_id_++;

    device.out_type_ = FNLog::DEVICE_OUT_FILE;
    device.config_fields_[FNLog::DEVICE_CFG_ABLE] = true;
    device.config_fields_[FNLog::DEVICE_CFG_PRIORITY] = level;
    device.config_fields_[FNLog::DEVICE_CFG_CATEGORY] = 0;
    device.config_fields_[FNLog::DEVICE_CFG_CATEGORY_EXTEND] = 0;
    device.config_fields_[FNLog::DEVICE_CFG_FILE_LIMIT_SIZE] = limit_size * 1024 * 1024;
    device.config_fields_[FNLog::DEVICE_CFG_FILE_ROLLBACK] = rollback;
    memcpy(device.out_path_, fpath.c_str(), fpath.size());
    memcpy(device.out_file_, fname.c_str(), fname.size());
    return 0;
}

int Logger::addLoggerToScreen(FNLog::LogPriority level) {
    FNLog::Logger& logger = FNLog::GetDefaultLogger();
    FNLog::Logger::StateLockGuard state_guard(logger.state_lock);
    if (logger.logger_state_ != FNLog::LOGGER_STATE_UNINIT) {
        perror("init logger failed");
        return -1;
    }
    if (logger.shm_ == nullptr) {
        perror("init logger error. no shm");
        return -2;
    }
    level_ = level;

    FNLog::Channel& channel = logger.shm_->channels_[0];
    FNLog::Device& device = channel.devices_[channel.device_size_++];
    memset(&device, 0, sizeof(device));
    device.device_id_ = device_id_++;

    device.out_type_ = FNLog::DEVICE_OUT_SCREEN;
    device.config_fields_[FNLog::DEVICE_CFG_ABLE] = true;
    device.config_fields_[FNLog::DEVICE_CFG_PRIORITY] = level;
    device.config_fields_[FNLog::DEVICE_CFG_CATEGORY] = 0;
    device.config_fields_[FNLog::DEVICE_CFG_CATEGORY_EXTEND] = 0;
    return 0;
}

int Logger::addLoggerToUdp(const std::string& ip, int port, FNLog::LogPriority level) {
    FNLog::Logger& logger = FNLog::GetDefaultLogger();
    FNLog::Logger::StateLockGuard state_guard(logger.state_lock);
    if (logger.logger_state_ != FNLog::LOGGER_STATE_UNINIT) {
        perror("init logger failed");
        return -1;
    }
    if (logger.shm_ == nullptr) {
        perror("init logger error. no shm");
        return -2;
    }

    FNLog::Channel& channel = logger.shm_->channels_[0];
    FNLog::Device& device = channel.devices_[channel.device_size_++];
    memset(&device, 0, sizeof(device));
    device.device_id_ = device_id_++;

    device.out_type_ = FNLog::DEVICE_OUT_UDP;
    device.config_fields_[FNLog::DEVICE_CFG_ABLE] = true;
    device.config_fields_[FNLog::DEVICE_CFG_PRIORITY] = level;
    device.config_fields_[FNLog::DEVICE_CFG_CATEGORY] = 0;
    device.config_fields_[FNLog::DEVICE_CFG_CATEGORY_EXTEND] = 0;
    device.config_fields_[FNLog::DEVICE_CFG_FILE_LIMIT_SIZE] = 1000 * 1000*1000;
    device.config_fields_[FNLog::DEVICE_CFG_FILE_ROLLBACK] = 4;
    device.config_fields_[FNLog::DEVICE_CFG_UDP_IP] = inet_addr(ip.c_str());
    device.config_fields_[FNLog::DEVICE_CFG_UDP_PORT] = htons(port);
    return 0;
}

