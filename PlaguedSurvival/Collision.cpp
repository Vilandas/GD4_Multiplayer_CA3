#include "RoboCatPCH.hpp"

/**
 * Vilandas Morrissey - D00218436
 */

 /// <summary>
 /// Tests for an intersection between two GameObjects. Predicts if two objects will collide.
 /// </summary>
 /// <param name="inDeltaTime"></param>
 /// <param name="a"></param>
 /// <param name="b"></param>
 /// <returns>True if nodes a & b intersect</returns>
bool Collision::Intersects(const float inDeltaTime, const GameObject& a, const GameObject& b)
{
	const sf::FloatRect aBounds = PredictMove(inDeltaTime, a);
	const sf::FloatRect bBounds = PredictMove(inDeltaTime, b);

	return aBounds.intersects(bBounds);
}

/// <summary>
/// Tests for the intersection location between two GameObjects. Predicts if two objects will collide.
/// </summary>
/// <param name="inDeltaTime"></param>
/// <param name="a"></param>
/// <param name="b"></param>
/// <returns>CollisionLocation/returns>
CollisionLocation Collision::CollisionLocation(const float inDeltaTime, const GameObject& a, const GameObject& b)
{
	const sf::FloatRect aBounds = PredictMove(inDeltaTime, a);
	const sf::FloatRect bBounds = PredictMove(inDeltaTime, b);

	if (aBounds.intersects(bBounds))
	{
		const float totalHalfWidth = (aBounds.width + bBounds.width) / 2;
		const float totalHalfHeight = (aBounds.height + bBounds.height) / 2;

		// distance from centre
		const sf::Vector2f distance = RoboMath::GetRectCenter(aBounds) - RoboMath::GetRectCenter(bBounds);

		const sf::Vector2f o = sf::Vector2f(
			std::ceil(totalHalfWidth - std::abs(distance.x)),
			std::ceil(totalHalfHeight - std::abs(distance.y))
		);

		if (o.x >= o.y)
		{
			return distance.y > 0 ? CollisionLocation::kTop : CollisionLocation::kBottom;
		}
		return distance.x > 0 ? CollisionLocation::kLeft : CollisionLocation::kRight;
	}

	return CollisionLocation::kNone;
}

sf::FloatRect Collision::PredictMove(float inDeltaTime, const sf::FloatRect& bounds, const sf::Vector2f& velocity)
{
	return {
		bounds.left + velocity.x,
		bounds.top + velocity.y,
		bounds.width,
		bounds.height
	};
}

sf::FloatRect Collision::PredictMove(float inDeltaTime, const GameObject& gameObject)
{
	const sf::FloatRect bounds = gameObject.GetBounds();
	const sf::Vector2f velocity = gameObject.GetVelocity() * inDeltaTime;

	return PredictMove(inDeltaTime, bounds, velocity);
}

