/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   daysOfWeek.cpp
 * Author: vermosen
 * 
 * Created on September 16, 2017, 3:06 PM
 */

#include "daysOfWeek.hpp"

template <>
const std::vector<std::pair<daysOfWeek, std::string> > enumConversions<daysOfWeek>::enumToStringVector = 
{
    { daysOfWeek::monday    , "Monday"      }
  , { daysOfWeek::tuesday   , "Tuesday"     }
  , { daysOfWeek::wednesday , "Wednesday"   }
  , { daysOfWeek::thursday  , "Thursday"    }
  , { daysOfWeek::friday    , "Friday"      }
  , { daysOfWeek::saturday  , "Saturday"    }
  , { daysOfWeek::sunday    , "Sunday"      }
};

