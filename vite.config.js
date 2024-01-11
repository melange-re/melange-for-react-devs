import { resolve } from 'path'
import { defineConfig } from 'vite'
import { nodeResolve } from '@rollup/plugin-node-resolve'

export default defineConfig({
  plugins: [nodeResolve()],
  server: {
    watch: {
      ignored: ['**/_opam']
    }
  },
  build: {
    rollupOptions: {
      input: {
        better_sandwiches: resolve(__dirname, 'src/better-sandwiches/index.html'),
      },
    },
  },
});
