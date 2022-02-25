/*
 * help.h
 *
 *  Created on: 21.02.2022
 *      Author: alexey slovesnov
 * copyright(c/c++): 2014-doomsday
 *           E-mail: slovesnov@yandex.ru
 *         homepage: slovesnov.users.sourceforge.net
 */

#ifndef HELP_H_
#define HELP_H_

#include "aslov.h"
#include <string>

const std::string storeFolder="tmp";

struct TagData{
	bool found;
	std::string content;
	size_t pos;
};
using VTagData = std::vector<TagData>;

/*
 * auto urls[]={
 * 	"https://online.globus.ru/catalog/aktsii/?PAGEN_1=1",
 * 	"https://online.globus.ru/catalog/aktsii/?PAGEN_1=12"
 * }
 * downloadFileInner works correct, return different results for urls[0] & urls[1]
 * downloadFileInner1, downloadFileInner2 return same result for both urls[0] & urls[1]
*/

std::pair<std::string,bool> downloadFileInner(std::string const& url);
//std::pair<std::string,bool> downloadFileInner1(std::string const& url);
//std::pair<std::string,bool> downloadFileInner2(std::string const& url);
//std::string downloadFile(std::string const& url);
std::string downloadFileAndSaveLocally(std::string const& url);
std::string getLocalFileName(std::string const& url);
std::string readLocalFile(std::string const& url);
std::string spanClass(std::string const& s);
TagData getTagContent(std::string const& s,size_t startpos,std::string const& tag);
VTagData getTagsContent(std::string const& s,size_t startpos,VString const& vtag);
std::string timeString(clock_t begin,bool total=false);

void init();
void deinit();
void mzipFile(std::string const &filepath);


#endif /* HELP_H_ */
