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
// and/or modify it under the terms of the GNU General License
// as published by the Free Software Foundation; version 2 of the License.
// Gurux Device Framework is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
// See the GNU General License for more details.
//
// More information of Gurux products: http://www.gurux.org
//
// This code is licensed under the GNU General License v2.
// Full text may be retrieved at http://www.gnu.org/licenses/gpl-2.0.txt
//---------------------------------------------------------------------------

#ifndef GXSETTINGS_H
#define GXSETTINGS_H

#include <vector>
#include "enums.h"
#include "GXByteBuffer.h"
#include "GXDLMSPlcRegister.h"
#include "GXDLMSPlcMeterInfo.h"

class CGXDLMSSettings;

// PLC communication settings.
class CGXPlcSettings
{
    CGXByteBuffer m_SystemTitle;
    CGXDLMSSettings* m_Settings;

    /**
     * Initial credit (IC) tells how many times the frame must be repeated.
     * Maximum value is 7.
     */
    unsigned char m_InitialCredit;
    /**
     * The current credit (CC) initial value equal to IC and automatically
     * decremented by the MAC layer after each repetition. Maximum value is 7.
     */
    unsigned char m_CurrentCredit;

    /**
     * Delta credit (DC) is used by the system management application entity
     * (SMAE) of the Client for credit management, while it has no meaning for a
     * Server or a REPEATER. It represents the difference(IC-CC) of the last
     * communication originated by the system identified by the DA address to
     * the system identified by the SA address. Maximum value is 3.
     */
    unsigned char m_DeltaCredit;
    /**
     * Source MAC address.
     */
    uint16_t m_MacSourceAddress;
    /**
     * Destination MAC address.
     */
    uint16_t m_MacDestinationAddress;
    /**
     * Response probability.
     */
    unsigned char m_ResponseProbability;
    /**
     * Allowed time slots.
     */
    uint16_t m_AllowedTimeSlots;
    /**
     * Server saves client system title.
     */
    CGXByteBuffer m_ClientSystemTitle;

public:
    /**
     * Initial credit (IC) tells how many times the frame must be
     *         repeated. Maximum value is 7.
     */
    unsigned char GetInitialCredit();
    void SetInitialCredit(unsigned char value);

    /**
     * The current credit (CC) initial value equal to IC and
     *         automatically decremented by the MAC layer after each repetition.
     *         Maximum value is 7.
     */
    unsigned char GetCurrentCredit();
    void SetCurrentCredit(unsigned char value);

    /**
     * Delta credit (DC) value.
     */
    unsigned char GetDeltaCredit();
    void SetDeltaCredit(unsigned char value);

    /**
     * IEC 61334-4-32 LLC uses 6 bytes long system title. IEC 61334-5-1
     *         uses 8 bytes long system title so we can use the default one.
     */
    CGXByteBuffer& GetSystemTitle();
    void SetSystemTitle(CGXByteBuffer& value);

    /**
     * Source MAC address.
     */
    uint16_t GetMacSourceAddress();
    void SetMacSourceAddress(uint16_t value);

    /**
     * Destination MAC address.
     */
    uint16_t GetMacDestinationAddress();
    void SetMacDestinationAddress(uint16_t value);

    /**
     * Response probability.
     */
    unsigned char GetResponseProbability();
    void SetResponseProbability(unsigned char value);

    /**
     * Allowed time slots.
     */
    uint16_t GetAllowedTimeSlots();
    void SetAllowedTimeSlots(uint16_t value);

    /**
     * Server saves client system title.
     */
    CGXByteBuffer& GetClientSystemTitle();
    void SetClientSystemTitle(CGXByteBuffer& value);

    //Reset settings to default values.
    void Reset();

    //Constructor.
    CGXPlcSettings(CGXDLMSSettings* settings);

    /**
     * Discover available  meters.
     *
     * @return Generated bytes.
     */
    int DiscoverRequest(CGXByteBuffer& data);

    /**
     * Generates discover report.
     *
     * systemTitle: System title
     * newMeter: Is this a new meter.
     * data: Generated bytes.
     * Returns error code.
     */
    int DiscoverReport(CGXByteBuffer& systemTitle, bool newMeter, CGXByteBuffer& data);

    /**
     * Parse discover reply.
     *
     * value: Received data.
     * sa: Source address.
     * da: Destination address.
     * list: Array of system titles and alarm descriptor error code
     */
    int ParseDiscover(CGXByteBuffer& value, uint16_t sa, uint16_t da, std::vector<CGXDLMSPlcMeterInfo>& list);

    /**
     * Register  meters.
     *
     * @param initiatorSystemTitle
     *            Active initiator system title
     * @param systemTitle SystemTitle
     * @param data Generated bytes
     * @return Error code.
     */
    int RegisterRequest(CGXByteBuffer& initiatorSystemTitle, CGXByteBuffer& systemTitle, CGXByteBuffer& data);

    /**
     * Parse register request.
     *
     * @return System title mac address.
     */
    int ParseRegisterRequest(CGXByteBuffer& value);

    /**
     * Parse discover request.
     *
     * data: Received data.
     * Returns: Error code.
     */
    int ParseDiscoverRequest(CGXByteBuffer& data, CGXDLMSPlcRegister& reg);

    /**
     * Ping  meter.
     *
     * data: Generated bytes.
     * Returns: Error code.
     */
    int PingRequest(CGXByteBuffer& systemTitle, CGXByteBuffer& data);

    /**
     * Parse ping response.
     *
     * data: Received data.
     * Returns: Error code.
     */
    int ParsePing(CGXByteBuffer& data, CGXByteBuffer& value);

    /**
     * Repear call request.
     *
     * data: Generated bytes.
     * Returns: Error code.
     */
    int RepeaterCallRequest(CGXByteBuffer& data);
};

#endif //GXSETTINGS_H
