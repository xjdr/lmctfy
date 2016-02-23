// Copyright 2014 Google Inc. All Rights Reserved.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#ifndef PRODUCTION_CONTAINERS_NSCON_CONFIGURATOR_USER_NS_CONFIGURATOR_H_
#define PRODUCTION_CONTAINERS_NSCON_CONFIGURATOR_USER_NS_CONFIGURATOR_H_

#include <sys/types.h>  // for pid_t

#include "base/macros.h"
#include "nscon/configurator/ns_configurator.h"
#include "util/task/status.h"
#include "util/task/statusor.h"
#include "google/protobuf/repeated_field.h"

namespace containers {
namespace nscon {

class IdMapEntry;
class NamespaceSpec;
class UserNsSpec;

// UserNsConfigurator
//
// This class implements system configuration specific to the USER namespace.
// For now, its enough to support configuring userns from 'outside'. So we dont
// override SetupInsideNamespace().
//
class UserNsConfigurator : public NsConfigurator {
 public:
  explicit UserNsConfigurator(NsUtil *ns_util)
      : NsConfigurator(CLONE_NEWUSER, ns_util) {}

  ~UserNsConfigurator() override {}

  ::util::Status SetupOutsideNamespace(const NamespaceSpec &spec,
                                       pid_t init_pid) const override;

 private:
  ::util::Status WriteIdMap(
      const string &id_map_path,
      const ::std::vector<IdMapEntry> &id_map) const;
  ::util::Status ProcSetGroupsWrite(pid_t child_pid) const;
  ::util::Status SetupUserNamespace(const UserNsSpec &user_spec,
                                    pid_t init_pid) const;
  ::util::StatusOr<::std::vector<IdMapEntry>> ValidateIdMap(
      const ::google::protobuf::RepeatedPtrField<IdMapEntry> &id_map) const;

  friend class UserNsConfiguratorTest;
  DISALLOW_COPY_AND_ASSIGN(UserNsConfigurator);
};

}  // namespace nscon
}  // namespace containers

#endif  // PRODUCTION_CONTAINERS_NSCON_CONFIGURATOR_USER_NS_CONFIGURATOR_H_
