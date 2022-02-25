/*
 * Platypus.cpp
 *
 *  Created on: 23.02.2022
 *      Author: alexey slovesnov
 * copyright(c/c++): 2014-doomsday
 *           E-mail: slovesnov@yandex.ru
 *         homepage: slovesnov.users.sourceforge.net
 */

#include "Platypus.h"

Platypus::Platypus() :
		GoodsParser( "https://www.utkonos.ru/cat", "/page/",__func__) {
}

void Platypus::parse0(const std::string &_s) {
	std::string s = _s,s1;
	int i;
	size_t p, p1;

	const std::string ta[] = { "<ul class=\"ng-star-inserted\">", "</ul>" };
	const std::string tb = "<div class=\"catalog-seo\">";
	const std::string tc = "<li class=\"ng-star-inserted\"><a href=\"";
	const std::string cat = "/cat";
	i = 0;
	std::map<size_t, int> map;
	p1 = s.find(tb, 0);
	assert(p1 != std::string::npos);
	p1 += tb.length();
	p = s.find("</div>",p1);
	assert(p != std::string::npos);
	s = s.substr(p1, p - p1);
	for (auto &t : ta) {
		p = 0;
		while ((p = s.find(t, p)) != std::string::npos) {
			p += t.length();
			map.insert( { p, !i });
		}
		i++;
	}

	i = 0;
	for (auto &a : map) {
		i += a.second ? 1 : -1;
		a.second = i;
	}

	p = 0;
	while ((p = s.find(tc, p)) != std::string::npos) {
		i = -1;
		p += tc.length();
		for (auto &a : map) {
			if (a.first > p) {
				break;
			}
			i = a.second;
		}
		if (i == 1) {
			p1 = s.find('"', p);
			s1=s.substr(p, p1 - p);
			assert(startsWith(s1, cat));
			m_vpages.push_back({m_stage0url+s1.substr(cat.length()),0});
			p = p1 + 1;
		}
	}

}

int Platypus::countPages(const std::string &s) {
	const std::string a=localeToUtf8("перейти на страницу");
	auto p=s.rfind(a);
	assert(p!=std::string::npos);
	return atoi(s.c_str()+p+a.length());
}

std::string Platypus::parseGoods(const std::string &s) {
	return "";
}
