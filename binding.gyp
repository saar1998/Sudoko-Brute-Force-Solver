{
	"targets": [{
	"target_name": "SudokoSolver",
	"cflags!": [ "-fno-exceptions" ],
	"cflags_cc!": [ "-fno-exceptions" ],
	"xcode_settings": {
    "GCC_ENABLE_CPP_EXCEPTIONS": "YES",
    "CLANG_CXX_LIBRARY": "libc++",
    "MACOSX_DEPLOYMENT_TARGET": "10.7",
  },
  'msvs_settings': {
    'VCCLCompilerTool': { 'ExceptionHandling': 1 },
  },
	"sources": [
			"main.cpp",
			"Sudoko_Brute_Force/Sudoko_Brute_Force.cpp"
	],
	'include_dirs': [
			"<!@(node -p \"require('node-addon-api').include\")"
	],
	'libraries': [],
	'dependencies': [
			"<!(node -p \"require('node-addon-api').gyp\")"
	],
	'defines': [ 'NAPI_DISABLE_CPP_EXCEPTIONS' ]
	}]
}