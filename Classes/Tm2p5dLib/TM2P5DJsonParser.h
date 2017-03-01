#ifndef TM2P5D_JSON_PARSER_H
#define TM2P5D_JSON_PARSER_H

#include "../../cocos2d/cocos/cocos2d.h"
#include "picojson.h"
#include "InfoClasses.h"

/**
 * TM2P5DJsonParser class is a member of the namespace 'TM2P5DComponent'.
 */
namespace TM2P5DComponent {

//Forward declarations
namespace Orientation { enum class Enum; }

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
	 * @return           [A root value of the json file]
	 */
	picojson::value parseJson(std::string json);

	/**
	 * Create information object from an object of json,and return it.
	 * @param  obj               [An object contains information]
	 * @return                   [An information object]
	 */
	MapInfo* comvJsonToMapInfo(picojson::value& obj);
	LayerInfo* comvJsonToLayerInfo(picojson::value& obj);
	LayerBundlerInfo* comvJsonToLayerBundlerInfo(picojson::value& obj);
	AtlasInfo* comvJsonToAtlasInfo(picojson::value& obj);

	/**
	 * Comvert a value in json to a type that is not supported in 'picojson'.
	 * @param  value  [A value of the string type in json]
	 */
	Orientation::Enum comvJsonValueToOrientation(picojson::value& value);
	cocos2d::Size comvJsonValueToCcsize(picojson::value& value);
	cocos2d::Rect comvJsonValueToCcRect(picojson::value& value);
};

} /* namespace TM2P5DComponent */

#endif
