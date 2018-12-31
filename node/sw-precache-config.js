module.exports = {
    staticFileGlobs: [
      'build/static/css/**.css',
      'build/static/js/**.js',
      'build/SkeletonModule.*'
    ],
    swFilePath: './build/service-worker.js',
    stripPrefix: 'build/',
    handleFetch: false,
  }