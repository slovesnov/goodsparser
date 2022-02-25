/*
 * Globus.h
 *
 *  Created on: 23.02.2022
 *      Author: alexey slovesnov
 * copyright(c/c++): 2014-doomsday
 *           E-mail: slovesnov@yandex.ru
 *         homepage: slovesnov.users.sourceforge.net
 */

#ifndef GLOBUS_H_
#define GLOBUS_H_

#include "GoodsParser.h"

class Globus: public GoodsParser {
public:
	const VString tagVIPrefix={""," ",".",localeToUtf8("руб. ")};
	VString tagVI;

	Globus();
	void parse0(std::string const& s)override;
	int countPages(std::string const&s)override;
	std::string parseGoods(std::string const& s)override;
};

#endif /* GLOBUS_H_ */
