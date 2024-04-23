import { resolve } from 'path'
import { defineConfig } from 'vite'
import { nodeResolve } from '@rollup/plugin-node-resolve'

export default defineConfig({
  base: '/demo/',
  // nodeResolve is needed to import melange runtime libraries in output
  // directory's `node_modules` directory. See
  // https://github.com/melange-re/melange-for-react-devs/pull/16#discussion_r1455989106
  plugins: [nodeResolve()],
  server: {
    watch: {
      ignored: ['**/_opam']
    }
  },
  build: {
    rollupOptions: {
      input: {
        'main': resolve(__dirname, 'index.html'),
        'counter': resolve(__dirname, 'src/counter/index.html'),
        'numeric-types': resolve(__dirname, 'src/numeric-types/index.html'),
        'celsius-converter-exception': resolve(__dirname, 'src/celsius-converter-exception/index.html'),
        'celsius-converter-option': resolve(__dirname, 'src/celsius-converter-option/index.html'),
        'order-confirmation': resolve(__dirname, 'src/order-confirmation/index.html'),
        'styling-with-css': resolve(__dirname, 'src/styling-with-css/index.html'),
        'better-sandwiches': resolve(__dirname, 'src/better-sandwiches/index.html'),
        'better-burgers': resolve(__dirname, 'src/better-burgers/index.html'),
        'sandwich-tests': resolve(__dirname, 'src/sandwich-tests/index.html'),
        'cram-tests': resolve(__dirname, 'src/cram-tests/index.html'),
        'burger-discounts': resolve(__dirname, 'src/burger-discounts/index.html'),
        'discounts-lists': resolve(__dirname, 'src/discounts-lists/index.html'),
        'promo-codes': resolve(__dirname, 'src/promo-codes/index.html'),
        'promo-component': resolve(__dirname, 'src/promo-component/index.html'),
        'coupon-codes': resolve(__dirname, 'src/coupon-codes/index.html'),
      },
    },
  },
});
