#pragma once

#include "Function.hpp"
#include "fix16.hpp"

// Maximum number of players in a (network) game
#define MAX_PLAYERS 6

class Player;
class Sprite;
class Car;
class Camera;
class Fix16_Rect;

namespace GameState
{
enum GameState
{
    ExecuteFrameAndPause_0 = 0,
    Running_1 = 1,
    Paused_2 = 2,
};
} // namespace GameState

namespace GameExitType
{
enum GameExitType
{
    None = 0,
    CloseGameImmediately_1 = 1,
    PlayerQuit_2 = 2,
    GameOverRIP_3 = 3,
    AreaCompleted_4 = 4,
    MultiplayerExit_5 = 5,
    ReplayExit_6 = 6,
};
} // namespace GameExitType

class Game
{
  public:
    // 9.6f 0x4A4750
    inline void ToggleFastForward_4A4750()
    {
        mFastForward = !mFastForward;
    }

    // 9.6f 0x416BC0
    inline bool IsPaused_416BC0() const
    {
        return mGameState == GameState::Paused_2;
    }

    // 9.6f 0x4B7590
    inline bool IsExitTimerSet_4B7590() const
    {
        return mExitTimer != -1;
    }

    // 9.6f 0x476790
    inline u8 GetLocalPlayerIdx_476790() const
    {
        return mLocalPlayerIdx;
    }

    // 9.6f 0x4D09C0
    inline s32 GetExitType_4D09C0() const
    {
        return mGameExitType;
    }

    EXPORT s32 GetFrameDurationMs() const;
    EXPORT void ExitGame(s32 new_timer, s32 exit_type, s8 bonus_type);
    EXPORT void ExitGameNoBonus(s32 new_timer, s32 exit_type);
    EXPORT bool UpdateExitTimer();
    EXPORT void LoadGameFiles();
    EXPORT void SetScreenSize(u32 width, u32 height);
    EXPORT void ApplyNetworkGameSettings();
    EXPORT void BootGame();
    EXPORT void ShowCounters();
    EXPORT void ShowDebugOverlay();
    EXPORT void Draw();
    EXPORT void UpdateAllPlayersControls();
    EXPORT void UpdatePaused();
    EXPORT void UpdateGame();
    EXPORT bool ExecuteGame();
    EXPORT void Pause();
    EXPORT void Unpause();
    EXPORT void TogglePause();
    EXPORT void SinglePlayerStepFrame();
    EXPORT void ReinitAfterVideoModeChange();
    EXPORT Player* GetFirstPlayerWithoutPed();
    EXPORT void ShakeCamerasAtPos(Fix16 amount, Fix16 x, Fix16 y);
    EXPORT bool IsSpriteInAnyPlayerBounds(Sprite* pSprite, Fix16 margin) const;
    EXPORT bool IsSpriteInBoundsOrFollowedByAnyPlayer(Sprite* pSprite, Fix16 margin) const;
    EXPORT bool IsSpriteInAnyPlayerView(Sprite* pSprite, s32 wide_view) const;
    EXPORT bool IsSpriteInPlayerView(Sprite* pSprite, u8 player_idx, s32 wide_view) const;
    EXPORT bool IsSpriteInPlayerBounds(Sprite* pSprite, u8 player_idx, Fix16 margin) const;
    EXPORT bool IsSpriteCameraSubjectForPlayer(Sprite* pSprite, u8 player_idx) const;
    EXPORT bool IsPointOnScreen(Fix16 x, Fix16 y);
    EXPORT bool IsRectVisibleToAnyPlayer(Fix16_Rect* pBounds);
    EXPORT Camera* IteratePlayerCamera();
    EXPORT bool IsCarInAnyPlayerHistory(Car* pCar);
    EXPORT Camera* IterateNextPlayerCamera();
    EXPORT Player* IterateFirstPlayer();
    EXPORT Player* IterateNextPlayer();
    EXPORT void ReduceTrafficSpawnCounterOfOtherViewers(Sprite* pSprite, Player* pExclude);
    EXPORT Game(u8 max_players, s8 player_idx); // 4B9DE0
    EXPORT ~Game(); // 4BAE30

    // 9.6f 0x4219E0
    Player* GetPlayer_4219E0(u8 idx) const
    {
        return mPlayers[idx];
    }

    s32 mGameState; // 0x0
    Player* mPlayers[MAX_PLAYERS]; // 0x4
    Player* mDrawPlayer; // 0x1C: set to mLocalPlayer in the ctor and never changed; only Draw reads it (gViewCamera)
    u8 mPlayerIterIdx; // 0x20
    u8 mCameraIterIdx; // 0x21
    bool mAuxCamIterated; // 0x22
    u8 mNumPlayers; // 0x23
    u8 mLocalPlayerIdx; // 0x24
    char_type mUnused25; // 0x25
    char_type mUnused26; // 0x26
    char_type mUnused27; // 0x27
    s32 mExitTimer; // 0x28
    s32 mGameExitType; // 0x2C
    bool mFastForward; // 0x30
    char_type mUnused31; // 0x31
    char_type mUnused32; // 0x32
    char_type mUnused33; // 0x33
    s32 mNetworkFrameMs; // 0x34
    Player* mLocalPlayer; // 0x38: mPlayers[mLocalPlayerIdx], the player controlled on this machine
    bool mSkipPolice; // 0x3C
    char_type mUnused3D; // 0x3D
    char_type mUnused3E; // 0x3E
    char_type mUnused3F; // 0x3F
};
GTA2_ASSERT_SIZEOF_ALWAYS(Game, 0x40);

EXTERN_GLOBAL(Game*, gGame_0x40);

EXTERN_GLOBAL(Fix16, kInitialAmbient);

EXTERN_GLOBAL(s32, gCachedScreenDimension);

EXTERN_GLOBAL(class CarChaseTaskTable*, gCarChaseTaskTable);
