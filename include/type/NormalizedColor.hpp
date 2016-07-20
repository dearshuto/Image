//
//  NormalizedColor.hpp
//  FUJI-Ray
//
//  Created by Shuto Shikama on 2016/02/27.
//
//

#ifndef NormalizedColor_hpp
#define NormalizedColor_hpp

#include <array>
#include <functional>

namespace fj {
    class NormalizedColor;
}

/**
 * 各要素が0~1であることが保証されている色
 */
class fj::NormalizedColor
{
public:
    static fj::NormalizedColor WHITE;
    static fj::NormalizedColor BLACK;
    static fj::NormalizedColor RED;
    static fj::NormalizedColor GREEN;
    static fj::NormalizedColor BLUE;
public:
    NormalizedColor()
    : NormalizedColor( 0.0, 0.0, 0.0)
    {
        
    }
    
    ~NormalizedColor() = default;
    
    NormalizedColor(const fj::NormalizedColor& other)
    : fj::NormalizedColor(other.r(), other.g(), other.b(), other.a())
    {
        // 初期化も移譲しているのでclamped()をよぶ必要はない
    }
    
    NormalizedColor(const double r, const double g, const double b)
    : NormalizedColor(r, g, b, 1.0)
    {
        // 初期化も移譲しているのでclamped()をよぶ必要はない
    }
    
    NormalizedColor(const double r, const double g, const double b, const double a)
    : m_value(std::array<double, 4>{{r, g, b, a}})
    {
        clamped();
    }
    
    NormalizedColor& operator=(const fj::NormalizedColor& other)
    {
        this->r() = other.r();
        this->g() = other.g();
        this->b() = other.b();
        this->a() = other.a();
        
        return std::ref(*this);
    }
    
    NormalizedColor operator+(const fj::NormalizedColor& other)const
    {
        const double kR = this->r() + other.r();
        const double kG = this->g() + other.g();
        const double kB = this->b() + other.b();
        const double kA = this->a() + other.a();
        NormalizedColor returnColor(kR, kG, kB, kA);
        
        returnColor.clamped();
        
        return returnColor;
    }
    
    NormalizedColor operator-(const fj::NormalizedColor& other)const
    {
        const double kR = this->r() - other.r();
        const double kG = this->g() - other.g();
        const double kB = this->b() - other.b();
        const double kA = this->a() - other.a();
        NormalizedColor returnColor(kR, kG, kB, kA);
        
        returnColor.clamped();
        
        return returnColor;
    }

    // 0~1の範囲を保っていると負の値は存在しないので, これらのoperatorは実装しても意味がない
    NormalizedColor operator-()const = delete;
    NormalizedColor operator-() = delete;
    
    NormalizedColor operator*(const double num)const
    {
        const double kR = this->r() * num;
        const double kG = this->g() * num;
        const double kB = this->b() * num;
        const double kA = this->a() * num;
        NormalizedColor returnColor(kR, kG, kB, kA);
        
        returnColor.clamped();
        
        return returnColor;
    }
    
    NormalizedColor operator/(const double num)const
    {
        return this->operator*(1.0 / num);
    }
    
    NormalizedColor& operator+=(const fj::NormalizedColor& other)
    {
        this->r() += other.r();
        this->g() += other.g();
        this->b() += other.b();
        this->a() += other.a();
        
        clamped();
        
        return std::ref(*this);
    }
    
    NormalizedColor& operator-=(const fj::NormalizedColor& other)
    {
        this->r() -= other.r();
        this->g() -= other.g();
        this->b() -= other.b();
        this->a() -= other.a();
        
        clamped();
        
        return std::ref(*this);
    }

    NormalizedColor& operator*=(const double num)
    {
        this->r() *= num;
        this->g() *= num;
        this->b() *= num;
        this->a() *= num;
        
        clamped();
        
        return std::ref(*this);
    }

    NormalizedColor& operator/=(const double num)
    {
        return this->operator*=(1.0 / num);
    }

    double operator[](const int index)
    {
        return std::ref(m_value[index]);
    }

    double operator[](const int index)const
    {
        return std::cref(m_value[index]);
    }
    
    /**
     * すべての値がゼロか調べる
     */
    bool isZero()const;
    
private:
    void clamped();
    void clamp(const int index);

public:
    
    double& r()
    {
        return std::ref(m_value[0]);
    }

    double& g()
    {
        return std::ref(m_value[1]);
    }

    double& b()
    {
        return std::ref(m_value[2]);
    }

    double& a()
    {
        return std::ref(m_value[3]);
    }

    double r()const
    {
        return std::cref(m_value[0]);
    }
    
    double g()const
    {
        return std::cref(m_value[1]);
    }
    
    double b()const
    {
        return std::cref(m_value[2]);
    }
    
    double a()const
    {
        return std::cref(m_value[3]);
    }

private:
    std::array<double, 4> m_value;
};

#endif /* NormalizedColor_hpp */
