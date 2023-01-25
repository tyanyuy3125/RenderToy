#include "texture.h"
#include "mathfunc.h"

#include <cmath>
#include <algorithm>

using namespace OpenPT;

void Fresnel(const Vector3f &I, const Vector3f &N, const float &ior, float &kr)
{
    float cosi = std::clamp(Vector3f::Dot(I, N), -1.0f, 1.0f);
    float etai = 1, etat = ior;
    if (cosi > 0)
    {
        std::swap(etai, etat);
    }
    // Compute sini using Snell's law
    float sint = etai / etat * sqrtf(std::max(0.f, 1 - cosi * cosi));
    // Total internal reflection
    if (sint >= 1)
    {
        kr = 1;
    }
    else
    {
        float cost = sqrtf(std::max(0.f, 1 - sint * sint));
        cosi = fabsf(cosi);
        float Rs = ((etat * cosi) - (etai * cost)) / ((etat * cosi) + (etai * cost));
        float Rp = ((etai * cosi) - (etat * cost)) / ((etai * cosi) + (etat * cost));
        kr = (Rs * Rs + Rp * Rp) / 2;
    }
    // As a consequence of the conservation of energy, transmittance is given by:
    // kt = 1 - kr;
}

namespace OpenPT
{

    OpenPT::Texture::Texture()
        : kd(Vector3f::O), emitivity(Vector3f::O)
    {
    }

    Texture::Texture(const Vector3f &reflectivity_, const Vector3f &emitivity_)
        : kd(reflectivity_), emitivity(emitivity_)
    {
    }

    const Vector3f Texture::Eval(const Vector3f &wi, const Vector3f &wo, const Vector3f &N) const
    {
        float cosalpha = Vector3f::Dot(N, wo);
        if (cosalpha > 0.0f)
        {
            auto ans = ks * CookTorrance(wi, wo, N) + kd * EvalDiffuse(wi, wo, N);
            //  clamp(0, 1, ans.x); clamp(0, 1, ans.y); clamp(0, 1, ans.z);
            return ans;
        }
        else
            return Vector3f(0.0f);
    }

    const Vector3f Texture::CookTorrance(const Vector3f &wi, const Vector3f &wo, const Vector3f &N) const
    {
        auto V = wo;
        auto L = wi;
        auto H = (V + L).Normalized();
        // auto type = m_type;

        if (!(Vector3f::Dot(N, V) > 0 && Vector3f::Dot(N, L) > 0))
            return 0;

        auto getFresnel = [&]()
        {
            static float n_air = 1, n_diff = 1.2, n_glos = 1.2;
            float n1 = n_air, n2;
            // if (type == MaterialType::MICROFACET_DIFFUSE)
            //     n2 = n_diff;
            // else
            //     n2 = n_glos;
            n2 = n_diff;
            auto costheta = Vector3f::Dot(N, V);
            float r0 = (n1 - n2) / (n1 + n2);
            r0 *= r0;
            return r0 + (1 - r0) * pow(1 - costheta, 5);
        };

        float F;
        float G, D; // Fresnel, Geometry, Distribution
        {
            // float F2 = getFresnel();
            Fresnel(wi, N, 1.2f, F);
            //    std::cout << F << " F " << F2 << std::endl;
            // F = F2;
        }
        {
            float G1 = 2 * Vector3f::Dot(N, H) * Vector3f::Dot(N, V) / Vector3f::Dot(V, H);
            float G2 = 2 * Vector3f::Dot(N, H) * Vector3f::Dot(N, L) / Vector3f::Dot(V, H);
            G = std::clamp(std::min(G1, G2), 0.0f, 1.0f);
        }
        {
            float m; // lager, more diffused
            // if (type == MaterialType::MICROFACET_DIFFUSE)
            //     m = 0.6;
            // else
            //     m = 0.2;
            m = 0.6;
            float alpha = acos(Vector3f::Dot(H, N));
            D = //(Vector3f::Dot(N,H) > 0) *
                exp(-pow(tan(alpha) / m, 2)) / (M_PI * m * m * pow(cos(alpha), 4));
        }
        auto ans = F * G * D / (Vector3f::Dot(N, L) * Vector3f::Dot(N, V) * 4);
        return ans;
    }

    const Vector3f Texture::EvalDiffuse(const Vector3f &wi, const Vector3f &wo, const Vector3f &N) const
    {
        return Vector3f(1.0 / M_PIf32);
    }
}