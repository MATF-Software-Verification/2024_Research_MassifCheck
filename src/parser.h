#ifndef PARSER_H
#define PARSER_H

#include "snapshot.h"
#include <string>
#include <vector>
#include <map>

std::pair<std::map<std::string, std::string>, std::vector<Snapshot>>parseMassifFile(const std::string &filePath);

#endif