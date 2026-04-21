/*
 * This file is part of the AzerothCore Project. See AUTHORS file for Copyright information
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef _SCRIPT_MGR_MACRO_H_
#define _SCRIPT_MGR_MACRO_H_

#include "ScriptMgr.h"

// ---------------------------------------------------------------------------
// Hook lifecycle annotation macros
//
// Usage example in a ScriptDefines header:
//
//   AC_HOOK_SINCE("1.0.0")
//   virtual void OnPlayerLogin(Player* player) { }
//
//   AC_HOOK_DEPRECATED("1.2.0", "Use OnPlayerCompleteLogin instead")
//   virtual void OnPlayerOldLogin(Player* player) { }
//
// These macros are no-ops at runtime — they exist purely as machine-readable
// documentation. A future CI lint step can grep for AC_HOOK_SINCE and verify
// that every public hook has an annotation.
// ---------------------------------------------------------------------------
#define AC_HOOK_SINCE(version)  /* introduced in AC Script API version version */
#define AC_HOOK_DEPRECATED(since_version, replacement) \
    [[deprecated("Deprecated since AC Script API " since_version ". " replacement)]]

template<typename ScriptName>
inline Optional<bool> IsValidBoolScript(std::function<bool(ScriptName*)> executeHook)
{
    if (ScriptRegistry<ScriptName>::ScriptPointerList.empty())
        return {};

    for (auto const& [scriptID, script] : ScriptRegistry<ScriptName>::ScriptPointerList)
    {
        if (executeHook(script))
            return true;
    }

    return false;
}

template<typename ScriptName, class T>
inline T* GetReturnAIScript(std::function<T*(ScriptName*)> executeHook)
{
    if (ScriptRegistry<ScriptName>::ScriptPointerList.empty())
        return nullptr;

    for (auto const& [scriptID, script] : ScriptRegistry<ScriptName>::ScriptPointerList)
    {
        if (T* scriptAI = executeHook(script))
        {
            return scriptAI;
        }
    }

    return nullptr;
}

template<typename ScriptName>
inline void ExecuteScript(std::function<void(ScriptName*)> executeHook)
{
    if (ScriptRegistry<ScriptName>::ScriptPointerList.empty())
        return;

    for (auto const& [scriptID, script] : ScriptRegistry<ScriptName>::ScriptPointerList)
    {
        executeHook(script);
    }
}

inline bool ReturnValidBool(Optional<bool> ret, bool need = false)
{
    return ret && *ret ? need : !need;
}

#define CALL_ENABLED_HOOKS(scriptType, hookType, action) \
    if (!ScriptRegistry<scriptType>::EnabledHooks[hookType].empty()) \
        for (auto const& script : ScriptRegistry<scriptType>::EnabledHooks[hookType]) { action; }

#define CALL_ENABLED_BOOLEAN_HOOKS(scriptType, hookType, action) \
    if (ScriptRegistry<scriptType>::EnabledHooks[hookType].empty()) \
        return true; \
    for (auto const& script : ScriptRegistry<scriptType>::EnabledHooks[hookType]) { if (action) return false; } \
    return true;

#define CALL_ENABLED_BOOLEAN_HOOKS_WITH_DEFAULT_FALSE(scriptType, hookType, action) \
    if (ScriptRegistry<scriptType>::EnabledHooks[hookType].empty()) \
        return false; \
    for (auto const& script : ScriptRegistry<scriptType>::EnabledHooks[hookType]) { if (action) return true; } \
    return false;

#endif // _SCRIPT_MGR_MACRO_H_
