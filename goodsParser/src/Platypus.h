/*
 * Platypus.h
 *
 *  Created on: 23.02.2022
 *      Author: alexey slovesnov
 * copyright(c/c++): 2014-doomsday
 *           E-mail: slovesnov@yandex.ru
 *         homepage: slovesnov.users.sourceforge.net
 */

#ifndef PLATYPUS_H_
#define PLATYPUS_H_

/* Note 25feb22 site https://www.utkonos.ru uses scripts for loading
 * goods so it's not possible at the moment load goods from this site
 */

#include "GoodsParser.h"

class Platypus: public GoodsParser {
public:
	Platypus();
	void parse0(std::string const& s)override;
	int countPages(std::string const&s)override;
	std::string parseGoods(std::string const& s)override;
};

#endif /* PLATYPUS_H_ */
