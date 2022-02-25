/*
 * Vprok.cpp
 *
 *  Created on: 25.02.2022
 *      Author: alexey slovesnov
 * copyright(c/c++): 2014-doomsday
 *           E-mail: slovesnov@yandex.ru
 *         homepage: slovesnov.users.sourceforge.net
 */

#include "Vprok.h"

Vprok::Vprok() : GoodsParser( "https://www.vprok.ru/catalog","",__func__) {
}

void Vprok::parse0(const std::string &s) {
}

int Vprok::countPages(const std::string &s) {
	return 0;
}

std::string Vprok::parseGoods(const std::string &s) {
	return "";
}
