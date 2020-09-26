#pragma once

#include <boost/shared_ptr.hpp>

#define SmartPointer( classname ) class classname; typedef boost::shared_ptr<classname> classname##Ptr
#define SmartPointers( structname ) struct structname; typedef boost::shared_ptr<structname> structname##Ptr
