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

//struct GoodData{
//	std::string url,data;
//};
using VGoodData = std::vector<PairStringString>;

class GoodsParser {
public:
	VBool m_useLocalFiles;
	bool m_useManyThreads;
	std::string m_stage0url,m_pageAdd,m_className;

	//rename
	std::vector<std::pair<std::string,int>> vp;
	std::vector<std::thread> vth;
	int threads;
	std::atomic_int atom;
	int upper;
	VGoodData vgoods;

	GoodsParser();
	void init(VBool useLocalFiles,bool useManyThreads);
	DownloadFunctionPtr getStageFunction(int stage);

	void stage0();
	void stage1();
	void stage2();

	virtual void parse0(std::string const&)=0;

	void th1(int t);
	void th2(int t);
	virtual int countPages(std::string const&)=0;
	virtual std::string parseGoods(std::string const& s)=0;
};

#endif /* GOODSPARSER_H_ */
