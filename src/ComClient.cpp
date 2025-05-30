#include "ComClient.h"

bool ComClient::WaitGetReady()
{
    return true;
}

GameSystem::Method ComClient::WaitReturnMethod([[maybe_unused]] GameSystem::AroundData data)
{
    return GameSystem::Method{GameSystem::TEAM::UNKNOWN, GameSystem::Method::ACTION::SEARCH,GameSystem::Method::ROTE::UP};
}

bool ComClient::WaitEndSharp([[maybe_unused]] GameSystem::AroundData data)
{
    return true;
}

void ComClient::Startup()
{
    emit Connected();
    emit WriteTeamName();
    emit Ready();
}

ComClient::ComClient(QObject *parent):
    BaseClient(parent)
{
    this->Name = "CPU";
    this->IP   = "ローカル";
}

ComClient::~ComClient()
{

}
