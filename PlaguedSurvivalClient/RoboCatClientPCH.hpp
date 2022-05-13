#include "RoboCatShared.hpp"

#include "functional"
#include "fstream"
#include "SFML\Graphics.hpp"

#include "Fonts.hpp"
#include "StateId.hpp"
#include "MusicThemes.hpp"

class StateStack;

#include "AnimatedSprite.hpp"
#include "AnimatedSpriteArtist.hpp"

#include "InputManager.hpp"
#include "SpriteComponent.hpp"
#include "PlayerSpriteComponent.hpp"
#include "TextureManager.hpp"
#include "MusicPlayer.hpp"
#include "FontManager.hpp"
#include "RenderManager.hpp"
#include "WindowManager.hpp"

#include "State.hpp"
#include "StateStack.hpp"

#include "ButtonType.hpp"
#include "Component.hpp"
#include "Container.hpp"
#include "Label.hpp"
#include "Button.hpp"

#include "CharacterType.hpp"
#include "DataTables.hpp"
#include "CharacterAnimationState.hpp"

#include "CharacterClient.hpp"
#include "TileClient.hpp"

#include "HUD.hpp"


#include "ReplicationManagerClient.hpp"
#include "NetworkManagerClient.hpp"
#include "Client.hpp"

#include "TitleState.hpp"
#include "MenuState.hpp"
#include "MultiplayerGameState.hpp"
#include "GameOverState.hpp"

