import { readFileSync } from "fs"
import { defineConfig } from 'vitepress'
import markdownItFootnote from 'markdown-it-footnote'

// From https://github.com/ocamllabs/vscode-ocaml-platform/blob/master/syntaxes/reason.json
const reasonGrammar = JSON.parse(readFileSync("./reason.json"))

// https://vitepress.dev/reference/site-config
export default defineConfig({
  title: "Melange for React Devs",
  description: "A project-based, guided introduction to Melange and its ecosystem for React developers",
  themeConfig: {
    search: {
      provider: 'local'
    },
    editLink: {
      pattern: 'https://github.com/melange-re/melange-for-react-devs/edit/develop/docs/:path'
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
          { text: 'Intro', link: '/intro' },
          { text: 'Installation', link: '/installation' },
          { text: 'Counter', link: '/counter/' },
          { text: 'Melange Playground', link: '/melange-playground' },
          { text: 'Celsius Converter', link: '/celsius-converter-exception/' },
          { text: 'Celsius Converter using Option', link: '/celsius-converter-option/' },
        ]
      }
    ],
    socialLinks: [
      { icon: 'github', link: 'https://github.com/melange-re/melange-for-react' }
    ],
  },
  markdown: {
    config: (md) => {
      md.set({ typographer: true })
      md.use(markdownItFootnote)
    },
    languages: [
      {
        id: "reason",
        scopeName: "source.reason",
        displayName: "Reason",
        grammar: reasonGrammar,
        aliases: ["re", "rei"],
      },
    ],
  },
})
