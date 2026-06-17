#pragma once

#define VGETTER(type, method, name)                                                        \
    /** Retrieves the name value for the object. @return The name value of the object. */      \
    const type&                                                                                       \
    Get##method() const                                                                        \
    {                                                                                          \
        return name;                                                                           \
    }

#define VGETTERSETTER(type, method, name)                                                      \
    /** Retrieves the name value for the object. @return The name value of the object. */      \
    const type&                                                                                       \
    Get##method() const                                                                        \
    {                                                                                          \
        return name;                                                                           \
    }                                                                                          \
    /** Sets the name value for the object. @param value The value of which to set name to. */ \
    virtual void Set##method(type value)                                                       \
    {                                                                                          \
        name = value;                                                                          \
    }

#define EVENTSOURCE(name, method, ...)                                           \
private:                                                                         \
    EventSource<__VA_ARGS__> name;                                               \
                                                                                 \
public:                                                                          \
    uint64_t Subscribe##method(const std::function<void(__VA_ARGS__)> &callback) \
    {                                                                            \
        return name.Subscribe(callback);                                         \
    }                                                                            \
    void Unsubscribe##method(uint64_t id)                                        \
    {                                                                            \
        name.Unsubscribe(id);                                                    \
    }                                                                            \
    void Trigger##method(__VA_ARGS__ args)                                       \
    {                                                                            \
        name.Trigger(args);                                                      \
    }

#define EVENTSOURCEV(name, method)                                    \
private:                                                              \
    EventSource<> name;                                               \
                                                                      \
public:                                                               \
    uint64_t Subscribe##method(const std::function<void()> &callback) \
    {                                                                 \
        return name.Subscribe(callback);                              \
    }                                                                 \
    void Unsubscribe##method(uint64_t id)                             \
    {                                                                 \
        name.Unsubscribe(id);                                         \
    }                                                                 \
    void Trigger##method()                                            \
    {                                                                 \
        name.Trigger();                                               \
    }
