
// Based classes
#include "berOctetString.h"
#include "berReal.h"
#include "berObjectIdentifier.h"
#include "berInteger.h"
#include "berVisibleString.h"
#include "berBoolean.h"
#include "berEnum.h"
#include "berNull.h"
#include "berBitString.h"

// Legacy classes
#include "berGeneralizedTime.h"
#include "berBMPString.h"
#include "berGeneralString.h"
#include "berGraphicString.h"
#include "berIA5String.h"
#include "berNumericString.h"
#include "berPrintableString.h"
#include "berTeletexString.h"
#include "berUniversalString.h"
#include "berUTF8String.h"
#include "berVideotexString.h"

#include "compositeValue.h"

#include "storages/berBaseType.h"

// Ber Variables
CBerIdentifier CBerInteger::s_Identifier(CBerIdentifier::UNIVERSAL_CLASS, CBerIdentifier::PRIMITIVE, CBerIdentifier::INTEGER_TAG);
CBerIdentifier CBerBitString::s_Identifier(CBerIdentifier::UNIVERSAL_CLASS, CBerIdentifier::PRIMITIVE, CBerIdentifier::BIT_STRING_TAG);
CBerIdentifier CBerBoolean::s_Identifier(CBerIdentifier::UNIVERSAL_CLASS, CBerIdentifier::PRIMITIVE, CBerIdentifier::BOOLEAN_TAG);
CBerIdentifier CBerNull::s_Identifier(CBerIdentifier::UNIVERSAL_CLASS, CBerIdentifier::PRIMITIVE, CBerIdentifier::NULL_TAG);
CBerIdentifier CBerReal::s_Identifier(CBerIdentifier::UNIVERSAL_CLASS, CBerIdentifier::PRIMITIVE, CBerIdentifier::REAL_TAG);
CBerIdentifier CBerObjectIdentifier::s_Identifier(CBerIdentifier::UNIVERSAL_CLASS, CBerIdentifier::PRIMITIVE, CBerIdentifier::OBJECT_IDENTIFIER_TAG);
CBerIdentifier CBerEnum::s_Identifier(CBerIdentifier::UNIVERSAL_CLASS, CBerIdentifier::PRIMITIVE, CBerIdentifier::ENUMERATED_TAG);

// Octet Strings
CBerIdentifier CBerOctetString::s_Identifier(CBerIdentifier::UNIVERSAL_CLASS, CBerIdentifier::PRIMITIVE, CBerIdentifier::OCTET_STRING_TAG);
CBerIdentifier CBerGeneralizedTime::s_Identifier(CBerIdentifier::UNIVERSAL_CLASS, CBerIdentifier::PRIMITIVE, CBerIdentifier::GENERALIZED_TIME_TAG);
CBerIdentifier CBerBMPString::s_Identifier(CBerIdentifier::UNIVERSAL_CLASS, CBerIdentifier::PRIMITIVE, CBerIdentifier::BMP_STRING_TAG);
CBerIdentifier CBerGeneralString::s_Identifier(CBerIdentifier::UNIVERSAL_CLASS, CBerIdentifier::PRIMITIVE, CBerIdentifier::GENERAL_STRING_TAG);
CBerIdentifier CBerGraphicString::s_Identifier(CBerIdentifier::UNIVERSAL_CLASS, CBerIdentifier::PRIMITIVE, CBerIdentifier::GRAPHIC_STRING_TAG);
CBerIdentifier CBerIA5String::s_Identifier(CBerIdentifier::UNIVERSAL_CLASS, CBerIdentifier::PRIMITIVE, CBerIdentifier::IA5_STRING_TAG);
CBerIdentifier CBerNumericString::s_Identifier(CBerIdentifier::UNIVERSAL_CLASS, CBerIdentifier::PRIMITIVE, CBerIdentifier::NUMERIC_STRING_TAG);
CBerIdentifier CBerPrintableString::s_Identifier(CBerIdentifier::UNIVERSAL_CLASS, CBerIdentifier::PRIMITIVE, CBerIdentifier::PRINTABLE_STRING_TAG);
CBerIdentifier CBerTeletexString::s_Identifier(CBerIdentifier::UNIVERSAL_CLASS, CBerIdentifier::PRIMITIVE, CBerIdentifier::TELETEX_STRING_TAG);
CBerIdentifier CBerUniversalString::s_Identifier(CBerIdentifier::UNIVERSAL_CLASS, CBerIdentifier::PRIMITIVE, CBerIdentifier::UNIVERSAL_STRING_TAG);
CBerIdentifier CBerUTF8String::s_Identifier(CBerIdentifier::UNIVERSAL_CLASS, CBerIdentifier::PRIMITIVE, CBerIdentifier::UTF8_STRING_TAG);
CBerIdentifier CBerVideotexString::s_Identifier(CBerIdentifier::UNIVERSAL_CLASS, CBerIdentifier::PRIMITIVE, CBerIdentifier::VIDEOTEX_STRING_TAG);
CBerIdentifier CBerVisibleString::s_Identifier(CBerIdentifier::UNIVERSAL_CLASS, CBerIdentifier::PRIMITIVE, CBerIdentifier::VISIBLE_STRING_TAG);

// Test Composite Class
CBerIdentifier CCompositeValue::s_Identifier(CBerIdentifier::UNIVERSAL_CLASS, CBerIdentifier::CONSTRUCTED, CBerIdentifier::PPDU_TAG);

// Ber Variables
quint32 CBerInteger::s_metaTypeId = qRegisterMetaType<CBerInteger*>();
quint32 CBerBitString::s_metaTypeId = qRegisterMetaType<CBerBitString*>();
quint32 CBerBoolean::s_metaTypeId = qRegisterMetaType<CBerBoolean*>();
quint32 CBerNull::s_metaTypeId = qRegisterMetaType<CBerNull*>();
quint32 CBerReal::s_metaTypeId = qRegisterMetaType<CBerReal*>();
quint32 CBerObjectIdentifier::s_metaTypeId = qRegisterMetaType<CBerObjectIdentifier*>();
quint32 CBerEnum::s_metaTypeId = qRegisterMetaType<CBerEnum*>();

// Octet Strings
quint32 CBerOctetString::s_metaTypeId = qRegisterMetaType<CBerOctetString*>();
quint32 CBerVideotexString::s_metaTypeId = qRegisterMetaType<CBerVideotexString*>();
quint32 CBerUTF8String::s_metaTypeId = qRegisterMetaType<CBerUTF8String*>();
quint32 CBerUniversalString::s_metaTypeId = qRegisterMetaType<CBerUniversalString*>();
quint32 CBerTeletexString::s_metaTypeId = qRegisterMetaType<CBerTeletexString*>();
quint32 CBerPrintableString::s_metaTypeId = qRegisterMetaType<CBerPrintableString*>();
quint32 CBerIA5String::s_metaTypeId = qRegisterMetaType<CBerIA5String*>();
quint32 CBerNumericString::s_metaTypeId = qRegisterMetaType<CBerNumericString*>();
quint32 CBerGraphicString::s_metaTypeId = qRegisterMetaType<CBerGraphicString*>();
quint32 CBerGeneralString::s_metaTypeId = qRegisterMetaType<CBerGeneralString*>();
quint32 CBerGeneralizedTime::s_metaTypeId = qRegisterMetaType<CBerGeneralizedTime*>();
quint32 CBerBMPString::s_metaTypeId = qRegisterMetaType<CBerBMPString*>();
quint32 CBerVisibleString::s_metaTypeId = qRegisterMetaType<CBerVisibleString*>();
quint32 CBerIdentifier::s_metaTypeId = qRegisterMetaType<CBerIdentifier*>();

quint32 CBerBaseStorage::s_metaTypeId = qRegisterMetaType<CBerBaseStorage*>();

// Test Composite Class
quint32 CCompositeValue::s_metaTypeIdentifier = qRegisterMetaType<CCompositeValue*>();

quint32 IBerBaseType::s_metaTypeId = qRegisterMetaType<IBerBaseType*>();
