#ifndef __GCOMMON_H__
#define __GCOMMON_H__
#include <thread>
#include <chrono>
#include <future>
#include <iostream>

#include "GWorld.h"
#include "GScene.h"

#include "GCom.h"
#include "GObj.h"

#include "GResourceMgr.h"
#include "GTransform.h"

#include <boost/log/sources/severity_feature.hpp>
#include <boost/log/trivial.hpp>
#include <boost/log/attributes/named_scope.hpp>
#include <boost/log/sources/global_logger_storage.hpp>
using namespace boost::log::trivial;
namespace src = boost::log::sources;
BOOST_LOG_INLINE_GLOBAL_LOGGER_DEFAULT(g_lg, src::severity_logger< severity_level >)

#endif