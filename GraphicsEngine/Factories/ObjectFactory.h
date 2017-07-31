#pragma once
#include"..\Graphics\GameObject.h"
#include"..\Core\PipelineType.h"
#include"..\Graphics\Axis.h"

class Scene;

/**
	ObjectCreate structure.
	Contains information needed to create a object.
*/
struct ObjectCreate
{
	glm::vec3 position;		//*< Position of a object.
	glm::vec3 scale;		//*< Scale factors for the object.
	float radians;			//*< Angle in radians by which to rotate the object.
	glm::vec3 rotationAxis;	//*< Axis by which to rotate the object.
	const char* mesh;		//*< File name of a mesh file.
	const char* texture;	//*< File name of the objects texture.
	const char* normalMap;	//*< File name of the normal map.
	const char* depthMap;	//*< File name of the depth map.
	PipelineType drawType;	//*< Pipeline with which to draw the object.
	int layer;				//*< Layer in which the object will be drawn.
};

/**
	BillboardCreate structure.
	Contains information needed to create a billboard object.
*/
struct BillboardCreate
{
	Scene* scene;			//*< Scene towards whose camera the object will be rotated.
	Axis axis;				//*< Defines around which axis the object will be rotated. 
	glm::vec3 position;		//*< Position of a object.
	glm::vec3 scale;		//*< Scale factors for the object.
	float radians;			//*< Angle in radians by which to rotate the object.
	glm::vec3 rotationAxis;	//*< Axis by which to rotate the object.
	const char* texture;	//*< File name of the objects texture.
	PipelineType drawType;	//*< Pipeline with which to draw the object.
	int layer;				//*< Layer in which the object will be drawn.
};

/**
	SliderCreate structure.
	Contains information needed to create a slider.
*/
struct SliderCreate
{	
	glm::vec4 position;			/**< Contains position and dimensions of a element. */
	const char* lineTexture;	/**< File name of a slider line background texture. */
	const char* sliderTexture;	/**< File name of a texture for the movable part of the slider. */	
	const char* optionName;		/**< Name of the option to which the slider is bound. */
};
/**
	BinderCreate structure.
	Contains information needed to create a binder.
*/
struct BinderCreate
{
	glm::vec4 position;			/**< Contains position and dimensions of a element. */
	glm::vec4 namePosition;		/**< Contains position and dimensions of a name relative to whole element. */	
	glm::vec4 qubePosition;		/**< Contains position and dimensions of a binder box relative to whole element. */	
	const char* nameTexture;	/**< File name of texture that represents the option. */	
	const char* qubeTexture;	/**< File name of texture that represents the box. */	
	const char* optionName;		/**< Name of the option to which the binder is bound. */
};
/**
	SelectorCreate structure.
	Contains information needed to create a selector.
*/
struct SelectorCreate
{
	glm::vec4 position;			/**< Contains position and dimensions of a element. */
	glm::vec4 namePosition;		/**< Contains position and dimensions of a name relative to whole element. */	
	glm::vec4 yesPosition;		/**< Contains position and dimensions of a yes option relative to whole element. */	
	glm::vec4 noPosition;		/**< Contains position and dimensions of a no option relative to whole element. */	
	const char* nameTexture;	/**< File name of texture that represents the selector name. */	
	const char* yesTexture;		/**< File name of texture that represents the yes option. */
	const char* noTexture;		/**< File name of texture that represents the no option. */	
	const char* optionName;		/**< Name of the option to which the selector is bound. */
};

/**
	ObjectFactory class
	Creates a different types of objects.
*/
class ObjectFactory
{
public:	
	/**
		Creates a object and returns it's pointer. Caller needs to delete the object.
		@param params structure with parameters needed to create a object.
		@return pointer to a game object. 
	*/
	static GameObject* createGameObject(const ObjectCreate& params);
	/**
		Creates an empty object and returns it's pointer. Caller needs to delete the object.
		@param position position of the object.
		@param scale scale of the object.
		@param radians angle in radians by which to rotate the object.
		@param rotationAxis axis by which to rotate the object.
	*/
	static GameObject* createEmpty(glm::vec3 position, glm::vec3 scale, float radians, glm::vec3 rotationAxis);
	/**
		Creates a rotating object and returns it's pointer. Caller needs to delete the object.
		@param params structure with parameters needed to create a object.
		@return pointer to a game object. 
	*/
	static GameObject* createGameObjectRotation(const ObjectCreate& params);
	/**
		Creates a skybox and returns it's pointer. Caller needs to delete the object.
		@param scene scene to which the skybox is bound.
		@param texture file name of a texture for the skybox.
		@return pointer to a game object. 
	*/
	static GameObject* createSkyBox(Scene* scene, const char* texture);
	/**
		Creates a billboard and returns it's pointer. Caller needs to delete the object.
		@param scene scene which will contain the billboard.
		@param texture filename of a billboard's texture.
		@param position initial position of a billboard object.
		@param layer in which the object will be drawn.
		@return pointer to a game object. 
	*/
	static GameObject* createBillboard(const BillboardCreate& params);
	/**
		Creates a slider and returns it's pointer. Caller needs to delete the object.
		Creates object is empty object which server as a container for parts of the slider.
		@param params structure with parameters needed to create a button.
		@return pointer to a game object. 
	*/
	static GameObject* createSlider(const SliderCreate& params);
	/**
		Creates a selector and returns it's pointer. Caller needs to delete the object.
		Creates object is empty object which server as a container for parts of the selector.
		@param params structure with parameters needed to create a selector.
		@return pointer to a game object. 
	*/
	static GameObject* createSelector(const SelectorCreate& params);
	/**
		Creates a binder and returns it's pointer. Caller needs to delete the object.
		Creates object is empty object which server as a container for parts of the binder.
		@param params structure with parameters needed to create a binder.
		@return pointer to a game object. 
	*/
	static GameObject* createBinder(const BinderCreate& params);
	friend class Djinn;
private:
	/**
		Creates a menu item and returns it's pointer. Caller needs to delete the object.
		@param position position of a menu item.
		@param texFilename name of menu item's a texture.
		@param pipeline pipeline with which to draw the menu item.
		@return pointer to a game object. 
	*/
	static GameObject* createMenuItem(const glm::vec4 & position, const char * texFilename, PipelineType pipeline);
	static GraphicsEngine* engine;
};