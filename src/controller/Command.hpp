#pragma once

template<class T>
class Command
{
public:
    virtual void execute(const T&);
};
