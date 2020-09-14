#ifndef MONSTER_FACTORY_H
#define MONSTER_FACTORY_H
#include "monster.h"
#include <functional>
#include <QJsonObject>
#include <map>
#include <QString>
#include <QDebug>

/**
 *  工厂类，用于创建
 */
class MonsterFactory
{
public:
    template <typename F>
    struct Register
    {
    public:
        Register(const QString &key)
        {
            qDebug() << key << " has been registered";
            MonsterFactory::get().m_map.emplace(
                key, [](int level) { return new F(level); });
            MonsterFactory::get().m_json_map.emplace(
                key, [](const QJsonObject &json) { return new F(json); });
        }
    };

    static Monster *produce(const QString &key, int level)
    {
        auto map = MonsterFactory::get().m_map;
        if (map.find(key) == map.end())
        {
            throw std::invalid_argument("error");
        }
        return map[key](level);
    }

    static Monster *fromJson(const QJsonObject &json)
    {
        QString key = json["name"].toString();
        auto map = MonsterFactory::get().m_json_map;
        if (map.find(key) == map.end())
        {
            throw std::invalid_argument("error");
        }
        return map[key](json);
    }

    static Monster *random(int level)
    {
        auto map = MonsterFactory::get().m_map;
        auto item = map.begin();
        std::advance(item, rand() % map.size());
        return (item->second)(level);
    }

    static QList<QString> monsters()
    {
        QList<QString> list;

        for (auto const &imap : MonsterFactory::get().m_map)
            list.push_back(imap.first);
        return list;
    }

private:
    MonsterFactory() = default;
    MonsterFactory(const MonsterFactory &) = delete;
    MonsterFactory(MonsterFactory &&) = delete;

    static MonsterFactory &get()
    {
        static MonsterFactory instance;
        return instance;
    }

    std::map<QString, std::function<Monster *(int)>> m_map;
    std::map<QString, std::function<Monster *(const QJsonObject &)>> m_json_map;
};

#define REGISTER_MONSTER_NAME(T) msg_name_##T##_
#define REGISTER_MONSTER(T, key, ...)                                \
    static MonsterFactory::Register<T> REGISTER_MONSTER_NAME(T)(key, \
                                                                ##__VA_ARGS__)
#endif // !MONSTER_FACTORY_H
