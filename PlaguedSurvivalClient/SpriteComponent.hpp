typedef shared_ptr< sf::Texture > TexturePtr;
typedef shared_ptr<sf::Font> FontPtr;

class SpriteComponent
{
public:

	SpriteComponent(GameObject* inGameObject);
	~SpriteComponent();


	void SetTexture(const TexturePtr& inTexture);
	virtual sf::Sprite& GetSprite();
	sf::RectangleShape GetDebugBounds() const;

protected:

	sf::Sprite mSprite;

	//don't want circular reference...
	GameObject* mGameObject;
};

typedef shared_ptr<SpriteComponent> SpriteComponentPtr;

