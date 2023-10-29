#pragma once

#include <function.h>

namespace Firework
{
    template
    <
        typename GetterReturnType,
        typename SetterInputType
    >
    struct Property
    {
        inline explicit Property(const std::pair<func::function<GetterReturnType()>, func::function<void(SetterInputType)>>&& constructor) :
        getter(std::move(std::get<0>(constructor))), setter(std::move(std::get<1>(constructor)))
        {
        }
        inline explicit Property(const Property<GetterReturnType, SetterInputType>& other) = delete;
        inline explicit Property(Property<GetterReturnType, SetterInputType>&& other) = delete;

        inline GetterReturnType operator=(SetterInputType value) const
        {
            this->setter(value);
            return this->getter();
        }
        inline GetterReturnType operator=(const Property<GetterReturnType, SetterInputType>& rhs) = delete;
        inline GetterReturnType operator=(Property<GetterReturnType, SetterInputType>&&) = delete;

        #pragma region Arithmetic
        inline auto operator+(auto rhs) const -> decltype(std::declval<GetterReturnType>() + rhs)
        requires requires (GetterReturnType _lhs, decltype(rhs) _rhs) { _lhs + _rhs; }
        {
            return (this->getter() + rhs);
        }
        inline GetterReturnType operator-(SetterInputType rhs) const
        {
            return (this->getter() - rhs);
        }
        inline GetterReturnType operator*(SetterInputType rhs) const
        {
            return (this->getter() * rhs);
        }
        inline GetterReturnType operator/(SetterInputType rhs) const
        {
            return (this->getter() / rhs);
        }
        inline GetterReturnType operator%(SetterInputType rhs) const
        {
            return (this->getter() % rhs);
        }
        
        inline GetterReturnType operator++() const
        {
            GetterReturnType type = this->getter();
            ++type;
            this->setter(type);
            return type;
        }
        inline GetterReturnType operator--() const
        {
            GetterReturnType type = this->getter();
            --type;
            this->setter(type);
            return type;
        }
        #pragma endregion

        #pragma region Assignment
        inline GetterReturnType operator+=(SetterInputType rhs) const
        {
            GetterReturnType type = this->getter();
            type += rhs;
            this->setter((SetterInputType)type);
            return this->getter();
        }
        inline GetterReturnType operator-=(SetterInputType rhs) const
        {
            GetterReturnType type = this->getter();
            type -= rhs;
            this->setter((SetterInputType)type);
            return this->getter();
        }
        inline GetterReturnType operator*=(SetterInputType rhs) const
        {
            GetterReturnType type = this->getter();
            type *= rhs;
            this->setter((SetterInputType)type);
            return this->getter();
        }
        inline GetterReturnType operator/=(SetterInputType rhs) const
        {
            GetterReturnType type = this->getter();
            type /= rhs;
            this->setter((SetterInputType)type);
            return this->getter();
        }
        inline GetterReturnType operator%=(SetterInputType rhs) const
        {
            GetterReturnType type = this->getter();
            type %= rhs;
            this->setter((SetterInputType)type);
            return this->getter();
        }
        #pragma endregion

        #pragma region Comparison
        inline GetterReturnType operator==(SetterInputType rhs) const
        {
            return this->getter() == rhs;
        }
        inline GetterReturnType operator!=(SetterInputType rhs) const
        {
            return this->getter() != rhs;
        }
        inline GetterReturnType operator>=(SetterInputType rhs) const
        {
            return this->getter() >= rhs;
        }
        inline GetterReturnType operator<=(SetterInputType rhs) const
        {
            return this->getter() <= rhs;
        }
        inline GetterReturnType operator>(SetterInputType rhs) const
        {
            return this->getter() > rhs;
        }
        inline GetterReturnType operator<(SetterInputType rhs) const
        {
            return this->getter() < rhs;
        }
        #pragma endregion

        inline GetterReturnType operator()() const
        {
            return this->getter();
        }
        inline operator GetterReturnType () const
        {
            return this->getter();
        }
    private:
        const func::function<GetterReturnType()> getter;
        const func::function<void(SetterInputType)> setter;
    };
    template <typename SetterInputType>
    struct Property<void, SetterInputType>
    {
        constexpr Property(auto&& setter) : setter(setter)
        { }

        constexpr void operator=(SetterInputType other) const
        {
            return this->setter(other);
        }
    private:
        const func::function<void(SetterInputType)> setter;
    };
}
