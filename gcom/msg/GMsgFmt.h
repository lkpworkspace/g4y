#ifndef __GMSGFMT_H__
#define __GMSGFMT_H__
#include <string>
#include <memory>

template<typename T>
class GMsgFmt
{
public:
    virtual std::string Serilize(std::shared_ptr<T>) = 0;
    virtual std::shared_ptr<T> Deserilize(std::string) = 0;
};

#endif