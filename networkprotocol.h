/// networkprotocol.h
#ifndef NETWORKPROTOCOL_H
#define NETWORKPROTOCOL_H

#include <QPointF>
#include <QPoint>
#include <QDataStream>
#include <QList>

enum MessageType : quint8 {
    Msg_Invalid = 0,
    Msg_GameState,
    Msg_PlayerInput,
    Msg_RoleAssignment,
    Msg_LevelCleared,
    Msg_PelletEaten,
    Msg_ResetLevel // Nouveau message
};

// Structure pour une commande de joueur
struct PlayerInput {
    int playerId;
    QPoint direction;
};

// Structure pour l'attribution de rôle
struct RoleAssignment {
    int playerId;
};

// Structure pour l'état d'un personnage
struct CharacterState {
    int id;
    QPointF position;
};

// Structure pour l'état complet du jeu à un instant T
struct GameState {
    QList<CharacterState> pacmanStates;
    QList<CharacterState> ghostStates;
    int score;
};

// Structure pour l'événement "bille mangée"
struct PelletEatenEvent {
    QPoint gridPosition;
};

// --- OPÉRATEURS DE SÉRIALISATION ---

inline QDataStream& operator<<(QDataStream& stream, const PlayerInput& input) {
    stream << input.playerId << input.direction;
    return stream;
}
inline QDataStream& operator>>(QDataStream& stream, PlayerInput& input) {
    stream >> input.playerId >> input.direction;
    return stream;
}

inline QDataStream& operator<<(QDataStream& stream, const RoleAssignment& role) {
    stream << role.playerId;
    return stream;
}
inline QDataStream& operator>>(QDataStream& stream, RoleAssignment& role) {
    stream >> role.playerId;
    return stream;
}

inline QDataStream& operator<<(QDataStream& stream, const CharacterState& state) {
    stream << state.id << state.position;
    return stream;
}
inline QDataStream& operator>>(QDataStream& stream, CharacterState& state) {
    stream >> state.id >> state.position;
    return stream;
}

inline QDataStream& operator<<(QDataStream& stream, const GameState& state) {
    stream << state.pacmanStates << state.ghostStates << state.score;
    return stream;
}
inline QDataStream& operator>>(QDataStream& stream, GameState& state) {
    stream >> state.pacmanStates >> state.ghostStates >> state.score;
    return stream;
}

inline QDataStream& operator<<(QDataStream& stream, const PelletEatenEvent& event) {
    stream << event.gridPosition;
    return stream;
}
inline QDataStream& operator>>(QDataStream& stream, PelletEatenEvent& event) {
    stream >> event.gridPosition;
    return stream;
}

#endif // NETWORKPROTOCOL_H
