
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

CBerIdentifier CBerOctetString::s_Identifier(CBerIdentifier::UNIVERSAL_CLASS, CBerIdentifier::PRIMITIVE, CBerIdentifier::OCTET_STRING_TAG);
CBerIdentifier CBerReal::s_Identifier(CBerIdentifier::UNIVERSAL_CLASS, CBerIdentifier::PRIMITIVE, CBerIdentifier::REAL_TAG);
CBerIdentifier CBerObjectIdentifier::s_Identifier(CBerIdentifier::UNIVERSAL_CLASS, CBerIdentifier::PRIMITIVE, CBerIdentifier::OBJECT_IDENTIFIER_TAG);
CBerIdentifier CBerInteger::s_Identifier(CBerIdentifier::UNIVERSAL_CLASS, CBerIdentifier::PRIMITIVE, CBerIdentifier::INTEGER_TAG);
CBerIdentifier CBerBitString::s_Identifier(CBerIdentifier::UNIVERSAL_CLASS, CBerIdentifier::PRIMITIVE, CBerIdentifier::BIT_STRING_TAG);
CBerIdentifier CBerVisibleString::s_Identifier(CBerIdentifier::UNIVERSAL_CLASS, CBerIdentifier::PRIMITIVE, CBerIdentifier::VISIBLE_STRING_TAG);
CBerIdentifier CBerBoolean::s_Identifier(CBerIdentifier::UNIVERSAL_CLASS, CBerIdentifier::PRIMITIVE, CBerIdentifier::BOOLEAN_TAG);
CBerIdentifier CBerEnum::s_Identifier(CBerIdentifier::UNIVERSAL_CLASS, CBerIdentifier::PRIMITIVE, CBerIdentifier::ENUMERATED_TAG);

CBerIdentifier CBerGeneralizedTime::s_Identifier(CBerIdentifier::UNIVERSAL_CLASS, CBerIdentifier::PRIMITIVE, CBerIdentifier::GENERALIZED_TIME_TAG);
CBerIdentifier CBerBMPString::s_Identifier(CBerIdentifier::UNIVERSAL_CLASS, CBerIdentifier::PRIMITIVE, CBerIdentifier::BMP_STRING_TAG);
CBerIdentifier CBerGeneralString::s_Identifier(CBerIdentifier::UNIVERSAL_CLASS, CBerIdentifier::PRIMITIVE, CBerIdentifier::GENERAL_STRING_TAG);
CBerIdentifier CBerGraphicString::s_Identifier(CBerIdentifier::UNIVERSAL_CLASS, CBerIdentifier::PRIMITIVE, CBerIdentifier::GRAPHIC_STRING_TAG);
CBerIdentifier CBerIA5String::s_Identifier(CBerIdentifier::UNIVERSAL_CLASS, CBerIdentifier::PRIMITIVE, CBerIdentifier::IA5_STRING_TAG);
CBerIdentifier CBerNull::s_Identifier(CBerIdentifier::UNIVERSAL_CLASS, CBerIdentifier::PRIMITIVE, CBerIdentifier::NULL_TAG);
CBerIdentifier CBerNumericString::s_Identifier(CBerIdentifier::UNIVERSAL_CLASS, CBerIdentifier::PRIMITIVE, CBerIdentifier::NUMERIC_STRING_TAG);
CBerIdentifier CBerPrintableString::s_Identifier(CBerIdentifier::UNIVERSAL_CLASS, CBerIdentifier::PRIMITIVE, CBerIdentifier::PRINTABLE_STRING_TAG);
CBerIdentifier CBerTeletexString::s_Identifier(CBerIdentifier::UNIVERSAL_CLASS, CBerIdentifier::PRIMITIVE, CBerIdentifier::TELETEX_STRING_TAG);
CBerIdentifier CBerUniversalString::s_Identifier(CBerIdentifier::UNIVERSAL_CLASS, CBerIdentifier::PRIMITIVE, CBerIdentifier::UNIVERSAL_STRING_TAG);
CBerIdentifier CBerUTF8String::s_Identifier(CBerIdentifier::UNIVERSAL_CLASS, CBerIdentifier::PRIMITIVE, CBerIdentifier::UTF8_STRING_TAG);
CBerIdentifier CBerVideotexString::s_Identifier(CBerIdentifier::UNIVERSAL_CLASS, CBerIdentifier::PRIMITIVE, CBerIdentifier::VIDEOTEX_STRING_TAG);
