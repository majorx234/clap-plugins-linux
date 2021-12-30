#include <cassert>
#include <cstring>
#include <filesystem>
#include <regex>

#include "path-provider.hh"

namespace clap {

   class LinuxPathProvider final : public PathProvider {
   public:
      LinuxPathProvider(const std::string &pluginPath, const std::string &pluginName)
         : prefix_(computePrefix(pluginPath)), pluginName_(pluginName) {}

      static std::string computePrefix(const std::string &pluginPath) {
         static const std::regex r("(/.*)/lib/clap/.*$", std::regex::optimize);

         std::smatch m;
         if (!std::regex_match(pluginPath, m, r))
            return "/usr";
         return m[1];
      }

      std::string getGuiExecutable() const override {
         return (prefix_ / "bin/clap-gui").generic_string();
      }

      std::string getSkinDirectory() const override {
         return (prefix_ / "lib/clap/qml" / pluginName_).generic_string();
      }

      std::string getQmlLibDirectory() const override {
         return (prefix_ / "lib/clap/qml").generic_string();
      }

      bool isValid() const noexcept override { return !prefix_.empty(); }

   private:
      const std::filesystem::path prefix_;
      const std::string pluginName_;
   };

   class LinuxDevelopmentPathProvider final : public PathProvider {
   public:
      LinuxDevelopmentPathProvider(const std::string &pluginPath, const std::string &pluginName)
         : srcRoot_(computeSrcRoot(pluginPath)), buildRoot_(computeBuildRoot(pluginPath)),
           pluginName_(pluginName), _multiPrefix(computeMultiPrefix(pluginPath)) {}

      static std::string computeSrcRoot(const std::string &pluginPath) {
         static const std::regex r("(/.*)/.*build.*/.*$", std::regex::optimize);

         std::smatch m;
         if (!std::regex_match(pluginPath, m, r))
            return "";
         return m[1];
      }

      static std::string computeBuildRoot(const std::string &pluginPath) {
         static const std::regex r("(/.*/.*build.*(/.*)?)/plugins/.*\\.clap$",
                                   std::regex::optimize);

         std::smatch m;
         if (!std::regex_match(pluginPath, m, r))
            return "";
         return m[1];
      }

      static std::string computeMultiPrefix(const std::string &pluginPath) {
         static const std::regex r("/.*/builds/[^/]*/plugins/([^/]*)/.*\\.clap$",
                                   std::regex::optimize);

         std::smatch m;
         if (!std::regex_match(pluginPath, r))
            return "";
         return m[1];
      }

      std::string getGuiExecutable() const override {
         return (buildRoot_ / "plugins/gui" / _multiPrefix / "clap-gui").generic_string();
      }

      std::string getSkinDirectory() const override {
         return (srcRoot_ / "plugins/qml" / pluginName_).generic_string();
      }

      std::string getQmlLibDirectory() const override {
         return (srcRoot_ / "plugins/qml").generic_string();
      }

      bool isValid() const noexcept override { return !srcRoot_.empty() && !buildRoot_.empty(); }

   private:
      const std::filesystem::path srcRoot_;
      const std::filesystem::path buildRoot_;
      const std::string pluginName_;
      const std::string _multiPrefix;
   };

   std::unique_ptr<PathProvider> PathProvider::create(const std::string &_pluginPath,
                                                      const std::string &pluginName) {
#ifdef __linux__
      {
         auto pluginPath = std::filesystem::absolute(_pluginPath);
         auto devPtr = std::make_unique<LinuxDevelopmentPathProvider>(pluginPath, pluginName);
         if (devPtr->isValid())
            return std::move(devPtr);

         auto ptr = std::make_unique<LinuxPathProvider>(pluginPath, pluginName);
         if (ptr->isValid())
            return std::move(ptr);
      }
#endif

      // TODO
      return nullptr;
   }
} // namespace clap