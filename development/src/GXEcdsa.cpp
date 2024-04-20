//
// --------------------------------------------------------------------------
//  Gurux Ltd
//
//
//
// Filename:        $HeadURL$
//
// Version:         $Revision$,
//                  $Date$
//                  $Author$
//
// Copyright (c) Gurux Ltd
//
//---------------------------------------------------------------------------
//
//  DESCRIPTION
//
// This file is a part of Gurux Device Framework.
//
// Gurux Device Framework is Open Source software; you can redistribute it
// and/or modify it under the terms of the GNU General Public License
// as published by the Free Software Foundation; version 2 of the License.
// Gurux Device Framework is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
// See the GNU General Public License for more details.
//
// More information of Gurux products: http://www.gurux.org
//
// This code is licensed under the GNU General Public License v2.
// Full text may be retrieved at http://www.gnu.org/licenses/gpl-2.0.txt
//---------------------------------------------------------------------------

#include "../include/GXEcdsa.h"
#include "../include/GXDLMSSha256.h"
#include "../include/GXDLMSSha384.h"

void CGXEcdsa::Multiply(CGXEccPoint& p,
    //Don't use reference for n. It's updated here.
    CGXBigInteger n,
    CGXBigInteger& N,
    CGXBigInteger& A,
    CGXBigInteger& P)
{
    CGXEccPoint p2 = JacobianMultiply(p, n, N, A, P);
    p.X = p2.X;
    p.Y = p2.Y;
    p.Z = p2.Z;
    FromJacobian(p, P);
}

void CGXEcdsa::JacobianAdd(CGXEccPoint& p,
    CGXEccPoint& q,
    CGXBigInteger& A,
    CGXBigInteger& P)
{
    if (!(p.Y.IsZero() || q.Y.IsZero()))
    {
        CGXBigInteger U1(p.X);
        U1.Multiply(q.Z);
        U1.Multiply(q.Z);
        U1.Mod(P);
        CGXBigInteger U2(p.Z);
        U2.Multiply(p.Z);
        U2.Multiply(q.X);
        U2.Mod(P);
        CGXBigInteger S1(p.Y);
        S1.Multiply(q.Z);
        S1.Multiply(q.Z);
        S1.Multiply(q.Z);
        S1.Mod(P);
        CGXBigInteger S2(q.Y);
        S2.Multiply(p.Z);
        S2.Multiply(p.Z);
        S2.Multiply(p.Z);
        S2.Mod(P);
        if (U1.Compare(U2) == 0)
        {
            if (S1.Compare(S2) != 0)
            {
                p.X = CGXBigInteger(0);
                p.Y = CGXBigInteger(0);
                p.Z = CGXBigInteger(1);
            }
            else
            {
                p.X = A;
                p.Y = P;
            }
        }
        //H
        CGXBigInteger H = U2;
        H.Sub(U1);
        //R
        CGXBigInteger R = S2;
        R.Sub(S1);
        CGXBigInteger H2(H);
        H2.Multiply(H);
        H2.Mod(P);
        CGXBigInteger H3(H);
        H3.Multiply(H2);
        H3.Mod(P);
        CGXBigInteger U1H2(U1);
        U1H2.Multiply(H2);
        U1H2.Mod(P);
        CGXBigInteger tmp(2);
        tmp.Multiply(U1H2);
        //nx
        CGXBigInteger nx(R);
        nx.Multiply(R);
        nx.Sub(H3);
        nx.Sub(tmp);
        nx.Mod(P);
        //ny
        CGXBigInteger ny = R;
        tmp = U1H2;
        tmp.Sub(nx);
        ny.Multiply(tmp);
        tmp = S1;
        tmp.Multiply(H3);
        ny.Sub(tmp);
        ny.Mod(P);
        //nz
        CGXBigInteger nz = H;
        nz.Multiply(p.Z);
        nz.Multiply(q.Z);
        nz.Mod(P);
        p.X = nx;
        p.Y = ny;
        p.Z = nz;
    }
}

void CGXEcdsa::FromJacobian(
    CGXEccPoint& p,
    CGXBigInteger& P)
{
    p.Z.Inv(P);
    p.X.Multiply(p.Z);
    p.X.Multiply(p.Z);
    p.X.Mod(P);
    p.Y.Multiply(p.Z);
    p.Y.Multiply(p.Z);
    p.Y.Multiply(p.Z);
    p.Y.Mod(P);
    p.Z.Clear();
}

CGXEccPoint CGXEcdsa::JacobianMultiply(
    CGXEccPoint& p,
    CGXBigInteger& n,
    CGXBigInteger& N,
    CGXBigInteger& A,
    CGXBigInteger& P)
{
    if (p.Y.IsZero() || n.IsZero())
    {
        return CGXEccPoint(0, 0, 1);
    }
    if (n.IsOne())
    {
        return p;
    }
    if (n.Compare(0) == -1 || n.Compare(N) != -1)
    {
        n.Mod(N);
        return JacobianMultiply(p, n, N, A, P);
    }
    if (n.IsEven())
    {
        n.Rshift(1);
        CGXEccPoint tmp2 = JacobianMultiply(p,
            n, N, A, P);
        CGXEccPoint value;
        JacobianDouble(tmp2, A, P, value);
        return value;
    }
    CGXEccPoint p2;
    n.Rshift(1);
    CGXEccPoint tmp2 = JacobianMultiply(p, n, N, A, P);
    JacobianDouble(tmp2, A, P, p2);
    JacobianAdd(p2, p, A, P);
    return p2;
}

int CGXEcdsa::JacobianDouble(
    CGXEccPoint& p,
    CGXBigInteger& A,
    CGXBigInteger& P,
    CGXEccPoint& value)
{
    CGXBigInteger ysq(p.Y);
    ysq.Pow(2);
    ysq.Mod(P);
    CGXBigInteger S(p.X);
    CGXBigInteger tmp2(4);
    S.Multiply(tmp2);
    S.Multiply(ysq);
    S.Mod(P);
    CGXBigInteger M(p.X);
    M.Pow(2);
    tmp2 = CGXBigInteger(3);
    M.Multiply(tmp2);
    CGXBigInteger tmp(p.Z);
    tmp.Pow(4);
    tmp.Multiply(A);
    M.Add(tmp);
    M.Mod(P);
    //nx
    CGXBigInteger nx(M);
    nx.Pow(2);
    tmp = S;
    CGXBigInteger bi(2);
    tmp.Multiply(bi);
    nx.Sub(tmp);
    nx.Mod(P);
    //ny
    CGXBigInteger ny(S);
    ny.Sub(nx);
    ny.Multiply(M);
    tmp = ysq;
    tmp.Pow(2);
    bi = CGXBigInteger(8);
    tmp.Multiply(bi);
    ny.Sub(tmp);
    ny.Mod(P);
    //nz
    CGXBigInteger nz(p.Y);
    nz.Multiply(p.Z);
    bi = CGXBigInteger(2);
    nz.Multiply(bi);
    nz.Mod(P);
    value = CGXEccPoint(nx, ny, nz);
    return 0;
}

int CGXEcdsa::GetRandomNumber(CGXBigInteger& N,
    CGXByteBuffer& value)
{
    value.SetSize(0);
    value.Capacity(4 * N.GetCount());
    unsigned char val;
    srand((unsigned int)time(NULL));
    for (uint16_t pos = 0; pos != (uint16_t)value.Capacity(); ++pos)
    {
        val = rand();
        if (val == 0 && pos == 0)
        {
            val = (1 + rand()) % 100;
        }
        value.SetUInt8(val);
    }
    return 0;
}

CGXEcdsa::CGXEcdsa(ECC scheme)
{
    m_Curve.Init(scheme);
}

int CGXEcdsa::GetSchemeSize(ECC scheme)
{
    return scheme == ECC_P256 ? 32 : 48;
}

CGXEcdsa::CGXEcdsa(CGXPublicKey& key) :
    CGXEcdsa(key.GetScheme())
{
    m_PublicKey = key;
}

CGXEcdsa::CGXEcdsa(CGXPrivateKey& key) :
    CGXEcdsa(key.GetScheme())
{
    m_PrivateKey = key;
}

int CGXEcdsa::Sign(CGXByteBuffer& data,
    CGXByteBuffer& signature)
{
    int ret;
    if (m_PrivateKey.GetRawValue().GetSize() == 0)
    {
        printf("Invalid private key.");
        return DLMS_ERROR_CODE_INVALID_PARAMETER;
    }
    if (m_PrivateKey.GetScheme() == ECC_P256)
    {
        if ((ret = CGXDLMSSha256::Hash(data, signature)) != 0)
        {
            return ret;
        }
    }
    else
    {
        if ((ret = CGXDLMSSha384::Hash(data, signature)) != 0)
        {
            return ret;
        }
    }
    CGXBigInteger msg(signature);
    signature.Clear();
    CGXByteBuffer tmp;
    CGXBigInteger pk(m_PrivateKey.m_RawValue.m_Data,
        (uint16_t)m_PrivateKey.m_RawValue.m_Size);
    CGXBigInteger n;
    CGXBigInteger r;
    CGXBigInteger s;
    do
    {
        ret = GetRandomNumber(m_Curve.m_N, tmp);
        if ((ret = s.Add(msg)) != 0)
        {
            break;
        }
        n = tmp;
        CGXBigInteger bi(1);
        CGXEccPoint p = CGXEccPoint(m_Curve.m_G.X,
            m_Curve.m_G.Y,
            bi);
        Multiply(p, n, m_Curve.m_N, m_Curve.m_A, m_Curve.m_P);
        r = p.X;
        r.Mod(m_Curve.m_N);
        n.Inv(m_Curve.m_N);
        //s
        s = CGXBigInteger(r);
        s.Multiply(pk);
        if ((ret = s.Add(msg)) != 0)
        {
            break;
        }
        s.Multiply(n);
        s.Mod(m_Curve.m_N);
    } while (r.IsZero() || s.IsZero());
    tmp.Clear();
    if (ret == 0 &&
        (ret = r.ToArray(signature, false)) == 0 &&
        (ret = s.ToArray(tmp, false)) == 0)
    {
        ret = signature.Set(tmp.GetData(), tmp.GetSize());
    }
    return ret;
}

int CGXEcdsa::GenerateSecret(
    CGXPublicKey& publicKey,
    CGXByteBuffer& secret)
{
    int ret;
    if (m_PrivateKey.GetRawValue().GetSize() == 0)
    {
        printf("Invalid private key.");
        ret = DLMS_ERROR_CODE_INVALID_PARAMETER;
    }
    else if (m_PrivateKey.GetScheme() != publicKey.GetScheme())
    {
        printf("Private key scheme is different than key.");
        ret = DLMS_ERROR_CODE_INVALID_PARAMETER;
    }
    else
    {
        CGXByteBuffer bb;
        CGXByteBuffer raw;
        publicKey.m_RawValue.ToByteBuffer(raw);
        int size = GetSchemeSize(m_PrivateKey.GetScheme());
        raw.SubArray(1, size, bb);
        CGXBigInteger x(bb);
        bb.SetSize(0);
        raw.SubArray(1 + size, size, bb);
        CGXBigInteger y(bb);
        CGXBigInteger pk(m_PrivateKey.m_RawValue.m_Data,
            (uint16_t)m_PrivateKey.m_RawValue.m_Size);
        CGXCurve curve;
        curve.Init(m_PrivateKey.GetScheme());
        CGXBigInteger bi(1);
        CGXEccPoint p(x, y, bi);
        p = JacobianMultiply(p, pk, m_Curve.m_N, m_Curve.m_A, m_Curve.m_P);
        FromJacobian(p, m_Curve.m_P);
        ret = p.X.ToArray(secret);
    }
    return ret;
}

int CGXEcdsa::GenerateKeyPair(
    ECC scheme,
    std::pair<CGXPublicKey, CGXPrivateKey>& kp)
{
    CGXPrivateKey pk;
    CGXPublicKey pub;
    CGXByteBuffer rn;
    CGXCurve curve;
    int ret;
    if ((ret = curve.Init(scheme)) == 0 &&
        (ret = GetRandomNumber(curve.m_N, rn)) == 0 &&
        (ret = CGXPrivateKey::FromRawBytes(rn, pk)) == 0 &&
        (ret = pk.GetPublicKey(pub)) == 0)
    {
        kp.first = pub;
        kp.second = pk;
    }
    return ret;
}

int CGXEcdsa::Verify(CGXByteBuffer& signature,
    CGXByteBuffer& data,
    bool& value)
{
    value = false;
    int ret;
    CGXByteBuffer bb;
    if (m_PublicKey.GetRawValue().GetSize() == 0)
    {
        if (m_PrivateKey.GetRawValue().GetSize() == 0)
        {
            printf("Invalid private key.");
            return DLMS_ERROR_CODE_INVALID_PARAMETER;
        }
        if ((ret = m_PrivateKey.GetPublicKey(m_PublicKey)) != 0)
        {
            return ret;
        }
    }
    if (m_PublicKey.GetScheme() == ECC_P256)
    {
        if ((ret = CGXDLMSSha256::Hash(data, bb)) != 0)
        {
            return ret;
        }
    }
    else
    {
        if ((ret = CGXDLMSSha384::Hash(data, bb)) != 0)
        {
            return ret;
        }
    }
    CGXBigInteger msg(bb);
    CGXByteBuffer pk;
    m_PublicKey.m_RawValue.ToByteBuffer(pk);
    bb = signature;
    CGXByteBuffer tmp;
    int size = GetSchemeSize(m_PublicKey.GetScheme());
    bb.SubArray(0, size, tmp);
    CGXBigInteger sigR(tmp);
    tmp.SetSize(0);
    bb.SubArray(size, size, tmp);
    CGXBigInteger sigS(tmp);
    CGXBigInteger inv = sigS;
    inv.Inv(m_Curve.m_N);
    // Calculate u1 and u2.
    CGXBigInteger bi(1);
    CGXEccPoint u1(m_Curve.m_G.X,
        m_Curve.m_G.Y,
        bi);
    CGXByteBuffer bb1, bb2;
    pk.SubArray(1, size, bb1);
    pk.SubArray(1 + size, size, bb2);
    CGXBigInteger bi1(bb1);
    CGXBigInteger bi2(bb2);
    bi = CGXBigInteger(1);
    CGXEccPoint u2(bi1,
        bi2,
        bi);
    CGXBigInteger n = msg;
    n.Multiply(inv);
    n.Mod(m_Curve.m_N);
    Multiply(u1, n, m_Curve.m_N, m_Curve.m_A, m_Curve.m_P);
    n = CGXBigInteger(sigR);
    n.Multiply(inv);
    n.Mod(m_Curve.m_N);
    Multiply(u2, n, m_Curve.m_N, m_Curve.m_A, m_Curve.m_P);
    u1.Z = CGXBigInteger(1);
    u2.Z = CGXBigInteger(1);
    JacobianAdd(u1, u2, m_Curve.m_A, m_Curve.m_P);
    FromJacobian(u1, m_Curve.m_P);
    value = sigR.Compare(u1.X) == 0;
    return 0;
}

int CGXEcdsa::Validate(CGXPublicKey& publicKey)
{
    int ret;
    if (publicKey.GetRawValue().GetSize() == 0)
    {
        printf("Invalid key.");
        ret = DLMS_ERROR_CODE_INVALID_PARAMETER;
    }
    else
    {
        CGXByteBuffer bb;
        int size = GetSchemeSize(publicKey.GetScheme());
        publicKey.GetRawValue().SubArray(1, size, bb);
        CGXBigInteger x(bb);
        bb.SetSize(0);
        publicKey.GetRawValue().SubArray(1 + size, size, bb);
        CGXBigInteger y(bb);
        CGXCurve curve;
        ret = curve.Init(publicKey.GetScheme());
        y.Multiply(y);
        y.Mod(curve.m_P);
        CGXBigInteger tmpX(x);
        tmpX.Multiply(x);
        tmpX.Mod(curve.m_P);
        tmpX.Add(curve.m_A);
        tmpX.Multiply(x);
        tmpX.Add(curve.m_B);
        tmpX.Mod(curve.m_P);
        if (y.Compare(tmpX) != 0)
        {
            printf("Public key validate failed. Public key is not valid ECDSA key.");
            ret = DLMS_ERROR_CODE_INVALID_PARAMETER;
        }
    }
    return ret;
}
