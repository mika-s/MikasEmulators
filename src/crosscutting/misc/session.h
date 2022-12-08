#pragma once

namespace emu::misc {

class Session {
public:
    virtual ~Session() = default;

    virtual void run() = 0;

    virtual void pause() = 0;

    virtual void stop() = 0;
};
}
