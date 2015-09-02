
#pragma once

// dependencies
#include <stddef.h>
#include <stdint.h>
#include <memory>
#include <string>
#include <string.h>
#include <type_traits>
#include <unordered_map>
#include <assert.h>

#define INCLUDING_REFLECTIVE

#include "reflective_settings.h"
#include "reflective_common.h"

// headers

#include "core\Identifier.h"
#include "core\PtrString.h"
#include "core\StringHash.h"
#include "core\Streams.h"

#include "type\Type.h"
#include "type\SpecialFunctions.h"
#include "type\StaticQualification.h"
#include "type\QualifiedTypeRef.h"

// inlines
#include "type\SpecialFunctions.inl"

#undef INCLUDING_REFLECTIVE
