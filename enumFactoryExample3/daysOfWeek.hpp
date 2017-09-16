/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   daysOfWeek.hpp
 * Author: vermosen
 *
 * Created on September 16, 2017, 3:06 PM
 */

#ifndef DAYSOFWEEK_HPP
#define DAYSOFWEEK_HPP

#include <enumManager.hpp>

enum class daysOfWeek { monday, tuesday, wednesday, thursday, friday, saturday, sunday };

template <>
const std::vector<std::pair<daysOfWeek, std::string>> enumFactory<daysOfWeek>::enumToStringVector;

#endif /* DAYSOFWEEK_HPP */

