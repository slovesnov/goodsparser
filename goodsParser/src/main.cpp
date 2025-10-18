/*
 * main.cpp
 *
 *       Created on: 01.09.2015
 *           Author: aleksey slovesnov
 * Copyright(c/c++): 2015-doomsday
 *           E-mail: slovesnov@yandex.ru
 *         Homepage: slovesnov.users.sourceforge.net
 */

#include "aslov.h"
#include "Globus.h"
#include "Vprok.h"
#include "Platypus.h"

//useLocalFiles for stages
#define LOCAL_TYPE 1

#if LOCAL_TYPE==0
const bool useLocalFiles[] = {0,0,0};
#elif LOCAL_TYPE==1
const bool useLocalFiles[] = {1,1,1};
#else
//other
const bool useLocalFiles[] = {0,0,0};
#endif
const bool useManyThreads=1;

/*
const int GLOBUS = 0;
const int VPROK = 1;
const int PLATYPUS = 2;
const int TYPE= 1;
*/

//#if TYPE==0 //true if TYPE is not defined so start from 1
#define GLOBUS 1
#define VPROK  2
#define PLATYPUS 3
#define TYPE 2

/*
 * globus totalTime=26:20
 */
int main(int argc, char *argv[]) {
	gtk_init(&argc, &argv); //do not remove
	setAllLocales();
	//aslovInit(argv);
	init();

	//std::string s, s1;

#if TYPE==GLOBUS
	Globus ob;
#elif TYPE==VPROK
	Vprok ob;
#elif TYPE==PLATYPUS
	Platypus ob;
#else
#error unknown TYPE
#endif

//	printl(ob.test());
//	return 0;


	ob.init(useLocalFiles,useManyThreads);

	println("%s threads=%d useLocalFiles={%s}",ob.m_className.c_str(),ob.m_threads,JOIN(useLocalFiles).c_str())

	//load categories
	ob.stage0();

	//load pages
	ob.stage1();
//
//	//load goods
//	ob.stage2();

	deinit();
}
