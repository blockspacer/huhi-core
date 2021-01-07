const config = require('../lib/config')
const util = require('../lib/util')
const path = require('path')
const fs = require('fs-extra')

const touchOverriddenFiles = () => {
  console.log('touch original files overridden by chromium_src...')

  // Return true when original file of |file| should be touched.
  const applyFileFilter = (file) => {
    // Exclude test files
    if (file.indexOf('browsertest') > -1 || file.indexOf('unittest') > -1) { return false }

    // Only includes cc and h files.
    const ext = path.extname(file)
    if (ext !== '.cc' && ext !== '.h' && ext !== '.mm') { return false }

    return true
  }

  const chromiumSrcDir = path.join(config.srcDir, 'huhi', 'chromium_src')
  var sourceFiles = util.walkSync(chromiumSrcDir, applyFileFilter)

  // Touch original files by updating mtime.
  const chromiumSrcDirLen = chromiumSrcDir.length
  sourceFiles.forEach(chromiumSrcFile => {
    var overriddenFile = path.join(config.srcDir, chromiumSrcFile.slice(chromiumSrcDirLen))
    if (!fs.existsSync(overriddenFile)) {
      // Try to check that original file is in gen dir.
      overriddenFile = path.join(config.outputDir, 'gen', chromiumSrcFile.slice(chromiumSrcDirLen))
    }

    if (fs.existsSync(overriddenFile)) {
      // If overriddenFile is older than file in chromium_src, touch it to trigger rebuild.
      if (fs.statSync(chromiumSrcFile).mtimeMs - fs.statSync(overriddenFile).mtimeMs > 0) {
        const date = new Date()
        fs.utimesSync(overriddenFile, date, date)
        console.log(overriddenFile + ' is touched.')
      }
    }
  })
}

const touchOverriddenVectorIconFiles = () => {
  console.log('touch original vector icon files overridden by huhi/vector_icons...')

  // Return true when original file of |file| should be touched.
  const applyFileFilter = (file) => {
    // Only includes icon files.
    const ext = path.extname(file)
    if (ext !== '.icon') { return false }
    return true
  }

  const huhiVectorIconsDir = path.join(config.srcDir, 'huhi', 'vector_icons')
  var huhiVectorIconFiles = util.walkSync(huhiVectorIconsDir, applyFileFilter)

  // Touch original files by updating mtime.
  const huhiVectorIconsDirLen = huhiVectorIconsDir.length
  huhiVectorIconFiles.forEach(huhiVectorIconFile => {
    var overriddenFile = path.join(config.srcDir, huhiVectorIconFile.slice(huhiVectorIconsDirLen))
    if (fs.existsSync(overriddenFile)) {
      // If overriddenFile is older than file in vector_icons, touch it to trigger rebuild.
      if (fs.statSync(huhiVectorIconFile).mtimeMs - fs.statSync(overriddenFile).mtimeMs > 0) {
        const date = new Date()
        fs.utimesSync(overriddenFile, date, date)
        console.log(overriddenFile + ' is touched.')
      }
    }
  })
}

/**
 * Checks to make sure the src/chrome/VERSION matches huhi-browser's package.json version
 */
const checkVersionsMatch = () => {
  const srcChromeVersionDir = path.resolve(path.join(config.srcDir, 'chrome', 'VERSION'))
  const versionData = fs.readFileSync(srcChromeVersionDir, 'utf8')
  const re = /MAJOR=(\d+)\s+MINOR=(\d+)\s+BUILD=(\d+)\s+PATCH=(\d+)/
  const found = versionData.match(re)
  const huhiVersionFromChromeFile = `${found[2]}.${found[3]}.${found[4]}`
  if (huhiVersionFromChromeFile !== config.huhiVersion) {
    // Only a warning. The CI environment will choose to proceed or not within its own script.
    console.warn(`Version files do not match!\nsrc/chrome/VERSION: ${huhiVersionFromChromeFile}\nhuhi-browser package.json version: ${config.huhiVersion}`)
  }
}

const build = (buildConfig = config.defaultBuildConfig, options) => {
  config.buildConfig = buildConfig
  config.update(options)
  checkVersionsMatch()

  touchOverriddenFiles()
  touchOverriddenVectorIconFiles()
  util.updateBranding()

  if (config.xcode_gen_target) {
    util.generateXcodeWorkspace()
  } else {
    util.buildTarget()
  }
}

module.exports = build
