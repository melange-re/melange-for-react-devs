import { resolve } from 'path'
import { defineConfig } from 'vite'
import { nodeResolve } from '@rollup/plugin-node-resolve'

export default defineConfig({
  // nodeResolve is needed to import melange runtime libraries in output
  // directory's `node_modules` directory. See
  // https://github.com/melange-re/melange-for-react-devs/pull/16#discussion_r1455989106
  plugins: [nodeResolve()],
  server: {
    watch: {
      ignored: ['**/_opam']
    }
  },
});
