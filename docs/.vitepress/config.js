import { readFileSync } from 'fs'
import { defineConfig } from 'vitepress'
import markdownItFootnote from 'markdown-it-footnote'

// https://vitepress.dev/reference/site-config
export default defineConfig({
  title: 'Melange for React Devs',
  description: 'A project-based, guided introduction to Melange and its ecosystem for React developers',
  base: '/',
  lastUpdated: true,
  themeConfig: {
    search: {
      provider: 'local'
    },
    editLink: {
      pattern: 'https://github.com/melange-re/melange-for-react-devs/edit/main/docs/:path'
    },
    nav: [
      { text: 'Home', link: '/' },
      { text: 'Melange', link: 'https://melange.re' },
      { text: 'ReasonReact', link: 'https://reasonml.github.io/reason-react/' }
    ],
    sidebar: [
      {
        text: 'Chapters',
        items: [
          { text: 'Intro', link: '/intro/' },
          { text: 'Installation', link: '/installation/' },
          { text: 'Counter', link: '/counter/' },
          { text: 'Numeric Types', link: '/numeric-types/' },
          { text: 'Celsius Converter', link: '/celsius-converter-exception/' },
          { text: 'Celsius Converter Using Option', link: '/celsius-converter-option/' },
          { text: 'Introduction to Dune', link: '/intro-to-dune/' },
          { text: 'Order Confirmation', link: '/order-confirmation/' },
          { text: 'Styling with CSS', link: '/styling-with-css/' },
          { text: 'Better Sandwiches', link: '/better-sandwiches/' },
          { text: 'Better Burgers', link: '/better-burgers/' },
          { text: 'Sandwich Tests', link: '/sandwich-tests/' },
          { text: 'Cram Tests', link: '/cram-tests/' },
          { text: 'Burger Discounts', link: '/burger-discounts/' },
          { text: 'Discounts Using Lists', link: '/discounts-lists/' },
          { text: 'Promo Codes', link: '/promo-codes/' },
          { text: 'Promo Component', link: '/promo-component/' },
          { text: 'Coupon Codes', link: '/coupon-codes/' },
        ]
      }
    ],
    socialLinks: [
      { icon: 'github', link: 'https://github.com/melange-re/melange-for-react-devs' }
    ],
  },
  markdown: {
    linkify: false,
    config: (md) => {
      md.set({ typographer: true })
      md.use(markdownItFootnote)
    },
    languages: [
      // Source: https://github.com/ocamllabs/vscode-ocaml-platform/blob/master/syntaxes/reason.json
      {
        id: 'reason',
        scopeName: 'source.reason',
        displayName: 'Reason',
        grammar: JSON.parse(readFileSync('./syntaxes/reason.json')),
        aliases: ['re', 'rei'],
      },
      // Source: https://github.com/ocamllabs/vscode-ocaml-platform/blob/master/syntaxes/dune.json
      // with a few additions to support melange.emit
      {
        id: 'dune',
        scopeName: 'source.dune',
        displayName: 'Dune',
        grammar: JSON.parse(readFileSync('./syntaxes/dune.json')),
      },
      // Source: https://github.com/ocamllabs/vscode-ocaml-platform/blob/master/syntaxes/cram.json
      {
        id: 'cram',
        scopeName: 'source.cram',
        displayName: 'Cram Test',
        grammar: JSON.parse(readFileSync('./syntaxes/cram.json')),
        aliases: ['t'],
      },
    ],
  },
})
