
[![Join the chat at https://gitter.im/turkdevops/Electron](https://badges.gitter.im/turkdevops/Electron.svg)](https://gitter.im/turkdevops/Electron?utm_source=badge&utm_medium=badge&utm_campaign=pr-badge&utm_content=badge)

```ts
// Welcome to the TypeScript Playground, this is a website
// which gives you a chance to write, share and learn TypeScript.

// You could think of it in three ways:
//
//  - A place to learn TypeScript in a place where nothing can break
//  - A place to experiment with TypeScript syntax, and share the URLs with others
//  - A sandbox to experiment with different compiler features of TypeScript

const anExampleVariable = "Hello World"
console.log(anExampleVariable)

// To learn more about the language, click above in "Examples" or "What's New".
// Otherwise, get started by removing these comments and the world is your playground.
  
```



<details><summary><b>Output</b></summary>

```ts
"use strict";
// Welcome to the TypeScript Playground, this is a website
// which gives you a chance to write, share and learn TypeScript.
// You could think of it in three ways:
//
//  - A place to learn TypeScript in a place where nothing can break
//  - A place to experiment with TypeScript syntax, and share the URLs with others
//  - A sandbox to experiment with different compiler features of TypeScript
const anExampleVariable = "Hello World";
console.log(anExampleVariable);
// To learn more about the language, click above in "Examples" or "What's New".
// Otherwise, get started by removing these comments and the world is your playground.

```


</details>


<details><summary><b>Compiler Options</b></summary>

```json
{
  "compilerOptions": {
    "noImplicitAny": true,
    "strictNullChecks": true,
    "strictFunctionTypes": true,
    "strictPropertyInitialization": true,
    "strictBindCallApply": true,
    "noImplicitThis": true,
    "noImplicitReturns": true,
    "alwaysStrict": true,
    "esModuleInterop": true,
    "declaration": true,
    "experimentalDecorators": true,
    "emitDecoratorMetadata": true,
    "moduleResolution": 2,
    "target": "ES2017",
    "jsx": "React",
    "module": "ESNext"
  }
}
```


</details>

**Playground Link:** [Provided](https://www.typescriptlang.org/play?#code/PTAEHUFMBsGMHsC2lQBd5oBYoCoE8AHSAZVgCcBLA1UABWgEM8BzM+AVwDsATAGiwoBnUENANQAd0gAjQRVSQAUCEmYKsTKGYUAbpGF4OY0BoadYKdJMoL+gzAzIoz3UNEiPOofEVKVqAHSKymAAmkYI7NCuqGqcANag8ABmIjQUXrFOKBJMggBcISGgoAC0oACCoASMFmgY7p7ehCTkVOle4jUMdRLYTqCc8LEZzCZmoNJODPHFZZXVtZYYkAAeRJTInDQS8po+rf40gnjbDKv8LqD2jpbYoACqAEoAMsK7sUmxkGSCc+VVQQuaTwVb1UBrDYULY7PagbgUZLJH6QbYmJAECjuMigZEMVDsJzCFLNXxtajBBCcQQ0MwAUVWDEQNUgADVHBQGNJ3KAALygABEAAkYNAMOB4GRogLFFTBPB3AExcwABT0xnM9zsyhc9wASmCKhwDQ8ZC8iElzhB7Bo3zcZmY7AYzEg-Fg0HUiS58D0Ii8AoZTJZggFSRxAvADlQAHJhAA5SASAVBFQAeW+ZF2gldWkgx1QjgUrmkeFATgtOlGWH0KAQiBhwiudokkuiIgMHBx3RYbC43CCJSAA)
      
[Playground Link](https://www.typescriptlang.org/play?ts=4.1.0-dev.20201028#code/PTAEBUE8AcFMGUDGAnAltALqAhgG2bNgCaSgDOArtNAPbIZmgYHYaoB2A5ju6LAB7ZEWMsw6dg7CgFsARrGSgAsACgQORtlC5UGBXgA0oAGZ0+g6dFywmAC1SNjFdsNQ1e73KTy4aAd0YMPxpVdQEhETEuTXYiUHYaUBoMWwUyAC5VVSJYRFxsAhNnV3c+dmxZawAxQgwKAgAKRBppaWxY9NAAIgIcslROdi7QAB9u9lg-AEF6BwwABWxOWC6ASk6ANxpUIgBuVVhyytga1nrYBp7YPoGh1f2VQ4rq2vOGgAMJ6dnRReX3+4HI4vM6NLpfRaQaSHDDwSCiWDSNYPUJgeBRbg6PTIPCMSjUOgMHC4XB2Gx+bCkSA0CigRDtUB+NB6HDKNRgURoLigDigACi8AATAAGEVGPy6WwQGAIFDoLAAFgAdABGRmwADkGxsAj0sWubPU+No9BMZg42No+TYpQ4-RyrL0lmtNk54m0un0uCVWRUGBloFOdQIjAAvN0AErXWD9QbDMbgyYzNi-JYrFHsiD2QLYADWMbJgdeIdAzjcvAqNG1RnacWY7TIpmQ0jIqL4Qil0AKWBoxkLZdKGESxDi7wAtLIcS5bO8cMZsYW3VxVP64EXQbAAEJTxBS8PvAAkAG8g+cyABfCc7mcZ9TKtXGwmaciwHt9r6gZYTNCIMc6fOgPmkDBMgRCMH49i7m21K0vSvAUGQNh2jsNhaE6VisDYWJekwiRtOw6AUC65AYmQPqZuAqSITgBCdAAqtQCj0ohRgADL+Ex2AsaAADC2DQLoeCoAAXqhsSgHRLj8YJOiib6q42KeBAAJIACKgPux7sX4nGIQAPEpMYAHyXjs7wPAp67BrAfLsBsABqBQaaAh5HgxcDIMxsAGcWxmXmp5m+uo6JcpwjC8gO7CaIUKA0GQjDSERbBWKg1xGGQiSoH2CEGtIdCwG2KQMu4NiRUwAYpIc7a7qWBGlNCcgKDyjCwBseAUJhRBtks2B2lghA1Q18iKMYyAtIWySpIokXkSuAa2c81zophYbdE8xxEPG3REA4i2bRZAaGXRykheIq2uYZF7NaAx4LRty16BegUqG2lEOGUlAlkV-XasgpC0PFqDHHSLSyBwrDlkkfYDbYbZle9QjNM4ehxE2s0Udm5VrnBZK8PIpaIXEEopDwPL6vwBqxuU1mFYkACOFCoIguZeKAbQAVoOiiFDgGwPCmQvX6AbwK+l3OUeqigKAADaADSZNWecakALqdLINA0NY7SqOet5gDx7hsFIWGEMgBHcnlhSVhQWDocR2E4rg4XsG24TOrA6RtIxRBjgpZBjsTti+4iGF6H+nqO62gvqAA8ooLB1qkPAJMUiIwqAlQ0NwAMYAL6i2BgGDQBkIA5BsmdhUq0hM2NGXzkqzTSMAftypgwDtCnLjiL7MpkK3GBjgqY4qhOr7YMAADEduYeH2J4D3cBkEAA)
