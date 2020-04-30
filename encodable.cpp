#include "encodable.h"
#include "protocolfield.h"
#include "protocolstructure.h"
#include "protocolcode.h"
#include "protocoldocumentation.h"
#include "protocolparser.h"

// Initialize convenience strings
const QString Encodable::VOID_ENCODE = "void encode";
const QString Encodable::INT_DECODE = "int decode";

/*!
 * Constructor for encodable
 */
Encodable::Encodable(ProtocolParser* parse, QString Parent, ProtocolSupport supported) :
    ProtocolDocumentation(parse, Parent, supported)
{
}


/*!
 * Check all names against C keyword and issue a warning if any of them match.
 * In addition the matching names will be updated to have a leading underscore
 */
void Encodable::checkAgainstKeywords(void)
{
    if(keywords.contains(name))
    {
        emitWarning("name matches C keyword, changed to _name");
        name = "_" + name;
    }

    if(keywords.contains(array))
    {
        emitWarning("array matches C keyword, changed to _array");
        array = "_" + array;
    }

    if(keywords.contains(variableArray))
    {
        emitWarning("variableArray matches C keyword, changed to _variableArray");
        variableArray = "_" + variableArray;
    }

    if(keywords.contains(array2d))
    {
        emitWarning("array2d matches C keyword, changed to _array2d");
        array2d = "_" + array2d;
    }

    if(keywords.contains(variable2dArray))
    {
        emitWarning("variable2dArray matches C keyword, changed to _variable2dArray");
        variable2dArray = "_" + variable2dArray;
    }

    if(keywords.contains(dependsOn))
    {
        emitWarning("dependsOn matches C keyword, changed to _dependsOn");
        dependsOn = "_" + dependsOn;
    }

    if(keywords.contains(dependsOnValue))
    {
        emitWarning("dependsOnValue matches C keyword, changed to _dependsOnValue");
        dependsOnValue = "_" + dependsOnValue;
    }

    if(variablenames.contains(name))
    {
        emitWarning("name matches ProtoGen variable, changed to _name");
        name = "_" + name;
    }

    if(variablenames.contains(array))
    {
        emitWarning("array matches ProtoGen variable, changed to _array");
        array = "_" + array;
    }

    if(variablenames.contains(variableArray))
    {
        emitWarning("variableArray matches ProtoGen variable, changed to _variableArray");
        variableArray = "_" + variableArray;
    }

    if(variablenames.contains(array2d))
    {
        emitWarning("array2d matches ProtoGen variable, changed to _array2d");
        array2d = "_" + array2d;
    }

    if(variablenames.contains(variable2dArray))
    {
        emitWarning("variable2dArray matches ProtoGen variable, changed to _variable2dArray");
        variable2dArray = "_" + variable2dArray;
    }

    if(variablenames.contains(dependsOn))
    {
        emitWarning("dependsOn matches ProtoGen variable, changed to _dependsOn");
        dependsOn = "_" + dependsOn;
    }

    if(variablenames.contains(dependsOnValue))
    {
        emitWarning("dependsOnValue matches ProtoGen variable, changed to _dependsOnValue");
        dependsOnValue = "_" + dependsOnValue;
    }

}// Encodable::checkAgainstKeywords


/*!
 * Reset all data to defaults
 */
void Encodable::clear(void)
{
    typeName.clear();
    name.clear();
    title.clear();
    comment.clear();
    array.clear();
    variableArray.clear();
    array2d.clear();
    variable2dArray.clear();
    encodedLength.clear();
    dependsOn.clear();
    dependsOnValue.clear();
    dependsOnCompare.clear();
}


/*!
 * Return the signature of this field in a encode function signature. The
 * string will start with ", " assuming this field is not the first part of
 * the function signature.
 * \return the string that provides this fields encode function signature
 */
QString Encodable::getEncodeSignature(void) const
{
    if(isNotEncoded() || isNotInMemory() || isConstant())
        return "";
    else if(is2dArray())
        return ", const " + typeName + " " + name + "[" + array + "][" + array2d + "]";
    else if(isArray())
        return ", const " + typeName + " " + name + "[" + array + "]";
    else if(isPrimitive())
        return ", " + typeName + " " + name;
    else
        return ", const " + typeName + "* " + name;
}


/*!
 * Return the signature of this field in a decode function signature. The
 * string will start with ", " assuming this field is not the first part of
 * the function signature.
 * \return the string that provides this fields decode function signature
 */
QString Encodable::getDecodeSignature(void) const
{
    if(isNotEncoded() || isNotInMemory())
        return "";
    else if(is2dArray())
        return ", " + typeName + " " + name + "[" + array + "][" + array2d + "]";
    else if(isArray())
        return ", " + typeName + " " + name + "[" + array + "]";
    else
        return ", " + typeName + "* " + name;
}


/*!
 * Return the string that documents this field as a encode function parameter.
 * The string starts with " * " and ends with a linefeed
 * \return the string that povides the parameter documentation
 */
QString Encodable::getEncodeParameterComment(void) const
{
    if(isNotEncoded() || isNotInMemory() || isConstant())
        return "";
    else
        return " * \\param " + name + " is " + comment + "\n";
}


/*!
 * Return the string that documents this field as a decode function parameter.
 * The string starts with " * " and ends with a linefeed
 * \return the string that povides the parameter documentation
 */
QString Encodable::getDecodeParameterComment(void) const
{
    if(isNotEncoded() || isNotInMemory())
        return "";
    else
        return " * \\param " + name + " receives " + comment + "\n";
}


/*!
 * Get a positive or negative return code string, which is language specific
 * \param positive should be true for "1" or "true", else "0", or "false"
 * \return The string in code that is the return.
 */
QString Encodable::getReturnCode(bool positive) const
{
    if(positive)
    {
        if(support.language == ProtocolSupport::c_language)
            return "1";
        else
            return "true";
    }
    else
    {
        if(support.language == ProtocolSupport::c_language)
            return "0";
        else
            return "false";
    }

}// Encodable::getReturnCode


/*!
 * Get the string which accessses this field in code in a encoding context.
 * \param isStructureMember true if this field is in the scope of a containing structure.
 * \return The string that accesses this field in code for reading.
 */
QString Encodable::getEncodeFieldAccess(bool isStructureMember) const
{
    return getEncodeFieldAccess(isStructureMember, name);

}// ProtocolField::getEncodeFieldAccess


/*!
 * Get the string which accessses this field in code in a encoding context.
 * \param isStructureMember true if this field is in the scope of a containing structure.
 * \param variable is the name of the variable to be accessed.
 * \return The string that accesses the variable in code for reading.
 */
QString Encodable::getEncodeFieldAccess(bool isStructureMember, const QString& variable) const
{
    QString access;

    // How we are going to access the field
    if(isStructureMember)
    {
        if(support.language == ProtocolSupport::c_language)
            access = "_pg_user->" + variable; // Access via structure pointer
        else
            access = variable;                // Access via implicit class reference
    }
    else
        access = variable;                    // Access via parameter

    // If the variable we are tyring to access is ourselves (i.e. not dependsOn
    // or variableArray, etc.) then we need to apply array access rules also.
    if(variable == name)
    {
        if(isArray() && !isString())
        {
            access += "[_pg_i]";
            if(is2dArray())
                access += "[_pg_j]";
        }

        // If we are a structure, and the language is C, we need the address of
        // the structure, even for encoding. Note however that if we are a
        // parameter we are already a pointer (because we never pass structures
        // by value).
        if(!isPrimitive() && (support.language == ProtocolSupport::c_language) && (isStructureMember || isArray()))
            access = "&" + access;
    }

    return access;

}// Encodable::getEncodeFieldAccess


/*!
 * Get the string which accessses this field in code in a decoding context.
 * \param isStructureMember true if this field is in the scope of a containing structure.
 * \return The string that accesses this field in code for writing.
 */
QString Encodable::getDecodeFieldAccess(bool isStructureMember) const
{
    return getDecodeFieldAccess(isStructureMember, name);
}


/*!
 * Get the string which accessses this field in code in a decoding context.
 * \param isStructureMember true if this field is in the scope of a containing structure.
 * \param variable is the name of the variable to be accessed.
 * \return The string that accesses this field in code for writing.
 */
QString Encodable::getDecodeFieldAccess(bool isStructureMember, const QString& variable) const
{
    QString access;

    if(isStructureMember)
    {
        if(support.language == ProtocolSupport::c_language)
            access = "_pg_user->" + variable; // Access via structure pointer
        else
            access = variable;                // Access via implicit class reference

        if(variable == name)
        {
            // Apply array access rules also, strings are left alone, they are already pointers
            if(isArray() && !isString())
            {
                access += "[_pg_i]";          // Array de-reference
                if(is2dArray())
                    access += "[_pg_j]";

            }

            // If we are a structure, and the language is C, we need the address of the structure.
            if(!isPrimitive() && (support.language == ProtocolSupport::c_language))
                access = "&" + access;
        }
    }
    else
    {
        if(variable == name)
        {
            if(isString())
                access = variable;                // Access via string pointer
            else if(isArray())
            {
                access = variable + "[_pg_i]";    // Array de-reference
                if(is2dArray())
                    access += "[_pg_j]";

                // If we are a structure, and the language is C, we need the address of the structure.
                if(!isPrimitive() && (support.language == ProtocolSupport::c_language))
                    access = "&" + access;
            }
            else if(!isPrimitive())
                access = variable;
            else
                access = "(*" + variable + ")";   // Access via parameter pointer
        }
        else
            access = "(*" + variable + ")";       // Access via parameter pointer
    }

    return access;

}// Encodable::getDecodeFieldAccess


/*!
 * Get the code that performs array iteration, in a encode context
 * \param spacing is the spacing that begins the first array iteration line
 * \param isStructureMember should be true if variable array limits are members of a structure
 * \return the code for array iteration, which may be empty
 */
QString Encodable::getEncodeArrayIterationCode(const QString& spacing, bool isStructureMember) const
{
    QString output;

    if(isArray())
    {
        if(variableArray.isEmpty())
        {
            output += spacing + "for(_pg_i = 0; _pg_i < " + array + "; _pg_i++)\n";
        }
        else
        {
            output += spacing + "for(_pg_i = 0; _pg_i < (unsigned)" + getEncodeFieldAccess(isStructureMember, variableArray) + " && _pg_i < " + array + "; _pg_i++)\n";
        }

        if(is2dArray())
        {
            if(variable2dArray.isEmpty())
            {
                output += spacing + TAB_IN + "for(_pg_j = 0; _pg_j < " + array2d + "; _pg_j++)\n";
            }
            else
            {
                output += spacing + TAB_IN + "for(_pg_j = 0; _pg_j < (unsigned)" + getEncodeFieldAccess(isStructureMember, variable2dArray) + " && _pg_j < " + array2d + "; _pg_j++)\n";
            }
        }

    }

    return output;

}// Encodable::getEncodeArrayIterationCode


/*!
 * Get the code that performs array iteration, in a decode context
 * \param spacing is the spacing that begins the first array iteration line
 * \param isStructureMember should be true if variable array limits are members of a structure
 * \return the code for array iteration, which may be empty
 */
QString Encodable::getDecodeArrayIterationCode(const QString& spacing, bool isStructureMember) const
{
    QString output;

    if(isArray())
    {
        if(variableArray.isEmpty())
        {
            output += spacing + "for(_pg_i = 0; _pg_i < " + array + "; _pg_i++)\n";
        }
        else
        {
            output += spacing + "for(_pg_i = 0; _pg_i < (unsigned)" + getDecodeFieldAccess(isStructureMember, variableArray) + " && _pg_i < " + array + "; _pg_i++)\n";
        }

        if(is2dArray())
        {
            if(variable2dArray.isEmpty())
            {
                output += spacing + TAB_IN + "for(_pg_j = 0; _pg_j < " + array2d + "; _pg_j++)\n";
            }
            else
            {
                output += spacing + TAB_IN + "for(_pg_j = 0; _pg_j < (unsigned)" + getDecodeFieldAccess(isStructureMember, variable2dArray) + " && _pg_j < " + array2d + "; _pg_j++)\n";
            }
        }

    }

    return output;

}// Encodable::getDecodeArrayIterationCode


/*!
 * Get documentation repeat details for array or 2d arrays
 * \return The repeat details
 */
QString Encodable::getRepeatsDocumentationDetails(void) const
{
    QString repeats = "1";
    QString arrayLink;
    QString array2dLink;
    QString variableArrayLink;
    QString variable2dArrayLink;

    if(isArray())
    {
        arrayLink = parser->getEnumerationNameForEnumValue(array);

        if(arrayLink.isEmpty())
            arrayLink = array;
        else
            arrayLink = "["+array+"](#"+arrayLink+")";

        if(variableArray.isEmpty())
            variableArrayLink = parser->getEnumerationNameForEnumValue(variableArray);

        if(variableArrayLink.isEmpty())
            variableArrayLink = variableArray;
        else
            variableArrayLink = "["+variableArray+"](#"+variableArrayLink+")";
    }

    if(is2dArray())
    {
        array2dLink = parser->getEnumerationNameForEnumValue(array2d);

        if(array2dLink.isEmpty())
            array2dLink = array2d;
        else
            array2dLink = "["+array2d+"](#"+array2dLink+")";

        if(variable2dArray.isEmpty())
            variable2dArrayLink = parser->getEnumerationNameForEnumValue(variable2dArray);

        if(variable2dArrayLink.isEmpty())
            variable2dArrayLink = variable2dArray;
        else
            variable2dArrayLink = "["+variable2dArray+"](#"+variable2dArrayLink+")";
    }

    if(is2dArray())
    {
        if(variableArray.isEmpty() && variable2dArray.isEmpty())
            repeats = arrayLink+"*"+array2dLink;
        else
            repeats = variableArrayLink+"*"+variable2dArrayLink + ", up to " + arrayLink+"*"+array2dLink;
    }
    else if(isArray())
    {
        if(variableArray.isEmpty())
            repeats = arrayLink;
        else
            repeats = variableArrayLink + ", up to " + arrayLink;
    }

    return repeats;

}// Encodable::getRepeatsDocumentationDetails


/*!
 * Construct a protocol field by parsing a DOM element. The type of Encodable
 * created will be either a ProtocolStructure or a ProtocolField
 * \param parse points to the global protocol parser that owns everything
 * \param Parent is the hierarchical name of the objec which owns the newly created object
 * \param supported describes what the protocol can support
 * \param field is the DOM element to parse (including its children)
 * \return a pointer to a newly allocated encodable. The caller is
 *         responsible for deleting this object.
 */
Encodable* Encodable::generateEncodable(ProtocolParser* parse, QString Parent, ProtocolSupport supported, const QDomElement& field)
{
    Encodable* enc = NULL;

    if(field.tagName().contains("Structure", Qt::CaseInsensitive))
        enc = new ProtocolStructure(parse, Parent, supported);
    else if(field.tagName().contains("Data", Qt::CaseInsensitive))
        enc = new ProtocolField(parse, Parent, supported);
    else if(field.tagName().contains("Code", Qt::CaseInsensitive))
        enc = new ProtocolCode(parse, Parent, supported);

    if(enc != NULL)
    {
        enc->setElement(field);
        enc->parse();
    }

    return enc;
}

