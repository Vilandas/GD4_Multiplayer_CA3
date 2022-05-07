class TextureManager
{
public:
	static void StaticInit();

	static std::unique_ptr<TextureManager> sInstance;

	TexturePtr GetTexture(Textures inTexture);

private:
	TextureManager();

	bool CacheTexture(Textures inTextureName, const char* inFileName);
	void CacheTexturePattern(Textures startTexture, Textures lastTexture, const std::string& locationPrefix);

	unordered_map<Textures, TexturePtr> mNameToTextureMap;
};

