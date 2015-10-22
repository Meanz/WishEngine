
namespace Wish
{
	//Top
	wish_game_object::wish_game_object(const char* name) : wish_transform(name)
	{
		TransformType = WISH_TRANSFORM_TYPE_GAME_OBJECT;
	}

	wish_game_object::~wish_game_object()
	{
	}
}