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
        field_30_fast_forward = !field_30_fast_forward;
    }

    // 9.6f 0x416BC0
    inline bool IsPaused_416BC0() const
    {
        return field_0_game_state == GameState::Paused_2;
    }

    // 9.6f 0x4B7590
    inline bool IsExitTimerSet_4B7590() const
    {
        return field_28_exit_timer != -1;
    }

    // 9.6f 0x476790
    inline u8 GetLocalPlayerIdx_476790() const
    {
        return field_24_local_player_idx;
    }

    // 9.6f 0x4D09C0
    inline s32 GetExitType_4D09C0() const
    {
        return field_2C_game_exit_type;
    }

    EXPORT s32 GetFrameDurationMs_4B8BB0() const;
    EXPORT void ExitGame_4B8BD0(s32 new_timer, s32 exit_type, s8 bonus_type);
    EXPORT void ExitGameNoBonus_4B8C00(s32 new_timer, s32 exit_type);
    EXPORT bool UpdateExitTimer_4B8C20();
    EXPORT void LoadGameFiles_4B8C40();
    EXPORT void SetScreenSize_4B8E00(u32 width, u32 height);
    EXPORT void ApplyNetworkGameSettings_4B8E50();
    EXPORT void BootGame_4B8EB0();
    EXPORT void ShowCounters_4B8FF0();
    EXPORT void ShowDebugOverlay_4B9270();
    EXPORT void Draw_4B92D0();
    EXPORT void UpdateAllPlayersControls_4B9380();
    EXPORT void UpdatePaused_4B93C0();
    EXPORT void UpdateGame_4B9410();
    EXPORT bool ExecuteGame_4B9640();
    EXPORT void Pause_4B96B0();
    EXPORT void Unpause_4B96C0();
    EXPORT void TogglePause_4B9700();
    EXPORT void SinglePlayerStepFrame_4B9710();
    EXPORT void ReinitAfterVideoModeChange_4B9720();
    EXPORT Player* GetFirstPlayerWithoutPed_4B9750();
    EXPORT void ShakeCamerasAtPos_4B9790(Fix16 amount, Fix16 x, Fix16 y);
    EXPORT bool IsSpriteInAnyPlayerBounds_4B97E0(Sprite* pSprite, Fix16 margin) const;
    EXPORT bool IsSpriteInBoundsOrFollowedByAnyPlayer_4B9830(Sprite* pSprite, Fix16 margin) const;
    EXPORT bool IsSpriteInAnyPlayerView_4B9890(Sprite* pSprite, s32 wide_view) const;
    EXPORT bool IsSpriteInPlayerView_4B98E0(Sprite* pSprite, u8 player_idx, s32 wide_view) const;
    EXPORT bool IsSpriteInPlayerBounds_4B9950(Sprite* pSprite, u8 player_idx, Fix16 margin) const;
    EXPORT bool IsSpriteCameraSubjectForPlayer_4B9A10(Sprite* pSprite, u8 player_idx) const;
    EXPORT bool IsPointOnScreen_4B9A80(Fix16 x, Fix16 y);
    EXPORT bool IsRectVisibleToAnyPlayer_4B9B10(Fix16_Rect* pBounds);
    EXPORT Camera* IteratePlayerCamera_4B9BC0();
    EXPORT bool IsCarInAnyPlayerHistory_4B9C10(Car* pCar);
    EXPORT Camera* IterateNextPlayerCamera_4B9C50();
    EXPORT Player* IterateFirstPlayer_4B9CD0();
    EXPORT Player* IterateNextPlayer_4B9D10();
    EXPORT void ReduceTrafficSpawnCounterOfOtherViewers_4B9D60(Sprite* pSprite, Player* pExclude);
    EXPORT Game(u8 max_players, s8 player_idx); // 4B9DE0
    EXPORT ~Game(); // 4BAE30

    Player* GetPlayer_4219E0(u8 idx) const
    {
        return field_4_players[idx];
    }

    s32 field_0_game_state;
    Player* field_4_players[MAX_PLAYERS];
    Player* field_1C_draw_player; // set to field_38_local_player in the ctor and never changed; only Draw_4B92D0 reads it (gViewCamera_676978)
    u8 field_20_player_iter_idx;
    u8 field_21_camera_iter_idx;
    bool field_22_aux_cam_iterated;
    u8 field_23_num_players;
    u8 field_24_local_player_idx;
    char_type field_25_unused;
    char_type field_26_unused;
    char_type field_27_unused;
    s32 field_28_exit_timer;
    s32 field_2C_game_exit_type;
    bool field_30_fast_forward;
    char_type field_31_unused;
    char_type field_32_unused;
    char_type field_33_unused;
    s32 field_34_network_frame_ms;
    Player* field_38_local_player; // field_4_players[field_24_local_player_idx], the player controlled on this machine
    bool field_3C_skip_police;
    char_type field_3D_unused;
    char_type field_3E_unused;
    char_type field_3F_unused;
};
GTA2_ASSERT_SIZEOF_ALWAYS(Game, 0x40);

EXTERN_GLOBAL(Game*, gGame_0x40_67E008);

EXTERN_GLOBAL(Fix16, kInitialAmbient_67DCCC);

EXTERN_GLOBAL(s32, gCachedScreenDimension_706C58);

EXTERN_GLOBAL(class CarChaseTaskTable*, gCarChaseTaskTable_678E30);
