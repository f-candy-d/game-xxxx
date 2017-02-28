#ifndef TM2P5D_JSON_PARSER_H
#define TM2P5D_JSON_PARSER_H

#include "../../cocos2d/cocos/cocos2d.h"
#include "picojson.h"
#include "InfoClasses.h"

/**
 * TM2P5DJsonParser class is a member of the namespace 'TM2P5DComponent'.
 */
namespace TM2P5DComponent {

class TM2P5DJsonParser : public cocos2d::Ref
{
public:
	static TM2P5DJsonParser* create();

	/**
	 * Parse a origin information (.json) file using 'picojson'.
	 * Then parse other information files.
	 * @method parseOriginJson
	 * @param  origin          [The name of a origin file that will be parsed]
	 */
	void parseOriginJson(std::string origin);

	/**
	 * If an error occurred while parsing json file,return true,otherwise return false.
	 * @method isError
	 * @return [true/false]
	 */
	bool isError();

	/**
	 * Show a last error of 'picojson'.
	 * @method coutLastError
	 */
	void outputLastError();

	/**
	 * Getter functions that returns information objects.
	 */
	MapInfo* getMapInfo();
	LayerInfo* getLayerInfoByName(std::string name);
	LayerBundlerInfo* getLayerBundlerInfoByName(std::string name);
	AtlasInfo* getAtlasInfoByName(std::string name);

	/**
	 * Show data of information objects.
	 */
	static void debugLogOfMapInfo(MapInfo* info);
	static void debugLogOfLayerInfo(LayerInfo* info);
	static void debugLogOfLayerBundlerInfo(LayerBundlerInfo* info);
	static void debugLogOfAtlasInfo(AtlasInfo* info);

protected:
	TM2P5DJsonParser();
	~TM2P5DJsonParser();
	bool init();

private:
	/**
	 * After parsing files,put generated objects into the following variables.
	 * Regarding the each Maps : key is the name of Layer, LayerBundker, tile-atlas.
	 */
	MapInfo* mMapInfo;
	cocos2d::Map<std::string,LayerInfo*> mLayerInfoMap;
	cocos2d::Map<std::string,LayerBundlerInfo*> mLayerBundlerInfoMap;
	cocos2d::Map<std::string,AtlasInfo*> mAtlasInfoMap;

	/**
	 * Parse a information file (.json) using 'picojson'.
	 * @method parseJson
	 * @param  json      [The name of json file that will be parsed]
	 */
	void parseJson(std::string json);
};

} /* namespace TM2P5DComponent */

#endif
