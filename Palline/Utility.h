#pragma once
#include <sstream>

template <typename T> std::string NumberToString(T Number);
/// <summary>
/// Funzione che permette di convertire un numero qualsiasi in stringa
/// </summary>
/// <typeparam name="T"></typeparam>
/// <param name="Number"></param>
/// <returns></returns>
template <typename T>
std::string NumberToString(T Number)
{
    std::ostringstream ss;
    ss.clear();
    ss << Number;
    ss << "\0";
    return ss.str();
}

