/*
 * WWCacheManager.h
 *
 *  Created on: 2014年5月19日
 *      Author: wly
 */

#ifndef  __HD_DDZ_CONTROL_WWCDBMANAGER_WWCACHEMANAGER_H__
#define __HD_DDZ_CONTROL_WWCDBMANAGER_WWCACHEMANAGER_H__

#include "cocos2d.h"
#include "../../WWMacros.h"

USING_NS_CC;

class WWCacheManager : public Ref
{
public:
	WWCacheManager(void);
	virtual ~WWCacheManager(void);
	SINGLE_INSTANCE_FUNC(WWCacheManager);

	//清空所有的缓存信息
	void purgeAllCache(void);

	//一般使用在场景替换时
	void removeAllUnusedCache(void);

	//for CCSpriteFrameCache
public:
	  /** Adds multiple Sprite Frames from a plist file.
     * A texture will be loaded automatically. The texture name will composed by replacing the .plist suffix with .png
     * If you want to use another texture, you should use the addSpriteFramesWithFile(const std::string& plist, const std::string& textureFileName) method.
     * @js addSpriteFrames
     * @lua addSpriteFrames
     */
    void addSpriteFramesWithFile(const std::string& plist);

    /** Adds multiple Sprite Frames from a plist file. The texture will be associated with the created sprite frames.
     @since v0.99.5
     * @js addSpriteFrames
     * @lua addSpriteFrames
     */
    void addSpriteFramesWithFile(const std::string& plist, const std::string& textureFileName);

    /** Adds multiple Sprite Frames from a plist file. The texture will be associated with the created sprite frames. 
     * @js addSpriteFrames
     * @lua addSpriteFrames
     */
    void addSpriteFramesWithFile(const std::string& plist, Texture2D *texture);

    /** Adds an sprite frame with a given name.
     If the name already exists, then the contents of the old name will be replaced with the new one.
     */
    void addSpriteFrame(SpriteFrame *frame, const std::string& frameName);

    /** Purges the dictionary of loaded sprite frames.
     * Call this method if you receive the "Memory Warning".
     * In the short term: it will free some resources preventing your app from being killed.
     * In the medium term: it will allocate more resources.
     * In the long term: it will be the same.
     */
    void removeSpriteFrames();

    /** Removes unused sprite frames.
     * Sprite Frames that have a retain count of 1 will be deleted.
     * It is convenient to call this method after when starting a new Scene.
     */
    void removeUnusedSpriteFrames();

    /** Deletes an sprite frame from the sprite frame cache. */
    void removeSpriteFrameByName(const std::string& name);

    /** Removes multiple Sprite Frames from a plist file.
    * Sprite Frames stored in this file will be removed.
    * It is convenient to call this method when a specific texture needs to be removed.
    * @since v0.99.5
    */
    void removeSpriteFramesFromFile(const std::string& plist);

    /** Removes all Sprite Frames associated with the specified textures.
     * It is convenient to call this method when a specific texture needs to be removed.
     * @since v0.995.
     */
    void removeSpriteFramesFromTexture(Texture2D* texture);

    /** Returns an Sprite Frame that was previously added.
     If the name is not found it will return nil.
     You should retain the returned copy if you are going to use it.
     * @js getSpriteFrame
     * @lua getSpriteFrame
     */
    SpriteFrame* getSpriteFrameByName(const std::string& name);

	//for CCTextureCache
public:
	/**
     * @js NA
     * @lua NA
     */
    virtual std::string getDescription() const;

//    Dictionary* snapshotTextures();

    /** Returns a Texture2D object given an filename.
    * If the filename was not previously loaded, it will create a new Texture2D
    *  object and it will return it. It will use the filename as a key.
    * Otherwise it will return a reference of a previously loaded image.
    * Supported image extensions: .png, .bmp, .tiff, .jpeg, .pvr
    */
    Texture2D* addImage(const std::string &filepath);

    /* Returns a Texture2D object given a file image
    * If the file image was not previously loaded, it will create a new Texture2D object and it will return it.
    * Otherwise it will load a texture in a new thread, and when the image is loaded, the callback will be called with the Texture2D as a parameter.
    * The callback will be called from the main thread, so it is safe to create any cocos2d object from the callback.
    * Supported image extensions: .png, .jpg
    * @since v0.8
    */
    virtual void addImageAsync(const std::string &filepath, const std::function<void(Texture2D*)>& callback);
    
    /* Unbind a specified bound image asynchronous callback
     * In the case an object who was bound to an image asynchronous callback was destroyed before the callback is invoked,
     * the object always need to unbind this callback manually.
     * @since v3.1
     */
    virtual void unbindImageAsync(const std::string &filename);
    
    /* Unbind all bound image asynchronous load callbacks
     * @since v3.1
     */
    virtual void unbindAllImageAsync();

    /** Returns a Texture2D object given an Image.
    * If the image was not previously loaded, it will create a new Texture2D object and it will return it.
    * Otherwise it will return a reference of a previously loaded image.
    * The "key" parameter will be used as the "key" for the cache.
    * If "key" is nil, then a new texture will be created each time.
    */
    Texture2D* addImage(cocos2d::Image *image, const std::string &key);

    /** Returns an already created texture. Returns nil if the texture doesn't exist.
    @since v0.99.5
    */
    Texture2D* getTextureForKey(const std::string& key) const;

    /** Reload texture from the image file
    * If the file image hasn't loaded before, load it.
    * Otherwise the texture will be reloaded from the file image.
    * The "filenName" parameter is the related/absolute path of the file image.
    * Return true if the reloading is succeed, otherwise return false.
    */
    bool reloadTexture(const std::string& fileName);

    /** Purges the dictionary of loaded textures.
    * Call this method if you receive the "Memory Warning"
    * In the short term: it will free some resources preventing your app from being killed
    * In the medium term: it will allocate more resources
    * In the long term: it will be the same
    */
    void removeAllTextures();

    /** Removes unused textures
    * Textures that have a retain count of 1 will be deleted
    * It is convenient to call this method after when starting a new Scene
    * @since v0.8
    */
    void removeUnusedTextures();

    /** Deletes a texture from the cache given a texture
    */
    void removeTexture(Texture2D* texture);

    /** Deletes a texture from the cache given a its key name
    @since v0.99.4
    */
    void removeTextureForKey(const std::string &key);

    /** Output to CCLOG the current contents of this TextureCache
    * This will attempt to calculate the size of each texture, and the total texture memory in use
    *
    * @since v1.0
    */
    std::string getCachedTextureInfo() const;

    //wait for texture cahe to quit befor destroy instance
    //called by director, please do not called outside
    void waitForQuit();

	//for CCAnimationCache
public:
	/** Purges the cache. It releases all the CCAnimation objects and the shared instance.
    */
    static void purgeSharedAnimationCache(void);

     /** Adds a Animation with a name.
    */
    void addAnimation(Animation *animation, const std::string& name);

    /** Deletes a Animation from the cache.
     
     */
    void removeAnimation(const std::string& name);

    /** Returns a Animation that was previously added.
    If the name is not found it will return nil.
    You should retain the returned copy if you are going to use it.
    */
    Animation* getAnimation(const std::string& name);

    /** Adds an animation from an NSDictionary
     Make sure that the frames were previously loaded in the SpriteFrameCache.
     @param plist The path of the relative file,it use to find the plist path for load SpriteFrames.
     @since v1.1
     */
    void addAnimationsWithDictionary(const cocos2d::ValueMap& dictionary,const std::string& plist);

    /** Adds an animation from a plist file.
     Make sure that the frames were previously loaded in the SpriteFrameCache.
     @since v1.1
     * @js addAnimations
     * @lua addAnimations
     */
    void addAnimationsWithFile(const std::string& plist);
};

#endif
