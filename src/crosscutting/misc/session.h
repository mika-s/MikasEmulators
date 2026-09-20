#pragma once

namespace emu::misc {

class Session {
public:
    virtual ~Session() = default;

    Session(const Session&) = delete;
    auto operator=(const Session&) -> Session& = delete;
    Session(Session&&) = delete;
    auto operator=(Session&&) -> Session& = delete;

    virtual void run() = 0;

    virtual void pause() = 0;

    virtual void stop() = 0;

protected:
    Session() = default;
};
}
