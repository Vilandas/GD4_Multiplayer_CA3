//I take care of rendering things!

class RenderManager
{
public:

	static void StaticInit();
	static std::unique_ptr<RenderManager> sInstance;

	void Reset();

	void Render();
	void RenderComponents();

	//vert inefficient method of tracking scene graph...
	void AddComponent(SpriteComponent* inComponent);
	void RemoveComponent(SpriteComponent* inComponent);
	int GetComponentIndex(SpriteComponent* inComponent) const;

	void AddArtist(AnimatedSpriteArtist* inArtist, bool priorityRender = false);
	void RemoveArtist(AnimatedSpriteArtist* inArtist);
	int GetArtistIndex(AnimatedSpriteArtist* inArtist, bool priorityRender) const;

	void AddText(sf::Text* inText);
	void RemoveText(sf::Text* inText);
	int GetTextIndex(sf::Text* inText) const;

private:

	RenderManager();

	//this can't be only place that holds on to component- it has to live inside a GameObject in the world
	vector<SpriteComponent*> mComponents;
	vector<AnimatedSpriteArtist*> mArtists;
	vector<AnimatedSpriteArtist*> mPriorityArtists;
	vector<sf::Text*> mExternalTexts;

	sf::View view;


};


