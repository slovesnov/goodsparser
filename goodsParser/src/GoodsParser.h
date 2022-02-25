/*
 * GoodsParser.h
 *
 *  Created on: 23.02.2022
 *      Author: alexey slovesnov
 * copyright(c/c++): 2014-doomsday
 *           E-mail: slovesnov@yandex.ru
 *         homepage: slovesnov.users.sourceforge.net
 */

#ifndef GOODSPARSER_H_
#define GOODSPARSER_H_

#include <vector>
#include <string>
#include <thread>
#include <atomic>

#include "help.h"

using VBool = std::vector<bool>;
using DownloadFunctionPtr = std::string (*)(std::string const&);

using VGoodData = std::vector<PairStringString>;

class GoodsParser {
public:
	VBool m_useLocalFiles;
	bool m_useManyThreads;
	std::string m_stage0url,m_pageAdd,m_className;
	std::vector<std::pair<std::string,int>> m_vpages;
	std::vector<std::thread> m_vthreads;
	int m_threads;
	std::atomic_int m_atom;
	int m_upper;
	VGoodData m_vgoods;
	clock_t m_begin;

	GoodsParser(std::string const &stage0url, std::string const &pageAdd,
			std::string const &className);
	void init(VBool useLocalFiles,bool useManyThreads);
	DownloadFunctionPtr getStageFunction(int stage);

	void stage0();
	void stage1();
	void stage2();
	void th(int stage,int t);
	void st(int stage);

	virtual void parse0(std::string const&)=0;
	virtual int countPages(std::string const&)=0;
	virtual std::string parseGoods(std::string const& s)=0;

};

#endif /* GOODSPARSER_H_ */
