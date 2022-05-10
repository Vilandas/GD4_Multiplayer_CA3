/**
 * Vilandas Morrissey - D00218436
 */

class Collision
{
public:
	static bool Intersects(float inDeltaTime, const GameObject& a, const GameObject& b);
	static CollisionLocation CollisionLocation(float inDeltaTime, const GameObject& a, const GameObject& b);
	static sf::FloatRect PredictMove(float inDeltaTime, const sf::FloatRect& bounds, const sf::Vector2f& velocity);
	static sf::FloatRect PredictMove(float inDeltaTime, const GameObject& gameObject);
};