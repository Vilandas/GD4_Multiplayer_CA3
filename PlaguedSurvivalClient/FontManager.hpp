class FontManager
{
public:
	static void StaticInit();

	static std::unique_ptr<FontManager> sInstance;

	FontPtr GetFont(Fonts font);

private:
	FontManager();

	bool CacheFont(Fonts font, const char* inFileName);

	unordered_map<Fonts, FontPtr> mNameToFontMap;
};

