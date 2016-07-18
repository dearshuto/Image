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
    typedef double Scalar;
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
    : NormalizedColor( fj::Scalar(0), fj::Scalar(0), fj::Scalar(0))
    {
        
    }
    
    ~NormalizedColor() = default;
    
    NormalizedColor(const fj::NormalizedColor& other)
    : fj::NormalizedColor(other.r(), other.g(), other.b(), other.a())
    {
        // 初期化も移譲しているのでclamped()をよぶ必要はない
    }
    
    NormalizedColor(const fj::Scalar& r, const fj::Scalar& g, const fj::Scalar& b)
    : NormalizedColor(r, g, b, fj::Scalar(1))
    {
        // 初期化も移譲しているのでclamped()をよぶ必要はない
    }
    
    NormalizedColor(const fj::Scalar& r, const fj::Scalar& g, const fj::Scalar& b, const fj::Scalar& a)
    : m_value(std::array<fj::Scalar, 4>{{r, g, b, a}})
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
        const fj::Scalar kR = this->r() + other.r();
        const fj::Scalar kG = this->g() + other.g();
        const fj::Scalar kB = this->b() + other.b();
        const fj::Scalar kA = this->a() + other.a();
        NormalizedColor returnColor(kR, kG, kB, kA);
        
        returnColor.clamped();
        
        return returnColor;
    }
    
    NormalizedColor operator-(const fj::NormalizedColor& other)const
    {
        const fj::Scalar kR = this->r() - other.r();
        const fj::Scalar kG = this->g() - other.g();
        const fj::Scalar kB = this->b() - other.b();
        const fj::Scalar kA = this->a() - other.a();
        NormalizedColor returnColor(kR, kG, kB, kA);
        
        returnColor.clamped();
        
        return returnColor;
    }

    // 0~1の範囲を保っていると負の値は存在しないので, これらのoperatorは実装しても意味がない
    NormalizedColor operator-()const = delete;
    NormalizedColor operator-() = delete;
    
    NormalizedColor operator*(const fj::Scalar& num)const
    {
        const fj::Scalar kR = this->r() * num;
        const fj::Scalar kG = this->g() * num;
        const fj::Scalar kB = this->b() * num;
        const fj::Scalar kA = this->a() * num;
        NormalizedColor returnColor(kR, kG, kB, kA);
        
        returnColor.clamped();
        
        return returnColor;
    }
    
    NormalizedColor operator/(const fj::Scalar& num)const
    {
        return this->operator*(fj::Scalar(1) / num);
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

    NormalizedColor& operator*=(const fj::Scalar& num)
    {
        this->r() *= num;
        this->g() *= num;
        this->b() *= num;
        this->a() *= num;
        
        clamped();
        
        return std::ref(*this);
    }

    NormalizedColor& operator/=(const fj::Scalar& num)
    {
        return this->operator*=(fj::Scalar(1) / num);
    }

    fj::Scalar& operator[](const int index)
    {
        return std::ref(m_value[index]);
    }

    const fj::Scalar& operator[](const int index)const
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
    
    fj::Scalar& r()
    {
        return std::ref(m_value[0]);
    }

    fj::Scalar& g()
    {
        return std::ref(m_value[1]);
    }

    fj::Scalar& b()
    {
        return std::ref(m_value[2]);
    }

    fj::Scalar& a()
    {
        return std::ref(m_value[3]);
    }

    const fj::Scalar& r()const
    {
        return std::cref(m_value[0]);
    }
    
    const fj::Scalar& g()const
    {
        return std::cref(m_value[1]);
    }
    
    const fj::Scalar& b()const
    {
        return std::cref(m_value[2]);
    }
    
    const fj::Scalar& a()const
    {
        return std::cref(m_value[3]);
    }

private:
    std::array<fj::Scalar, 4> m_value;
};

#endif /* NormalizedColor_hpp */
