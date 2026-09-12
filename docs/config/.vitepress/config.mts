import { defineConfig } from 'vitepress'
import lightbox from 'vitepress-plugin-lightbox'
import { enLocale } from './locales/en.mts'
import { ruLocale } from './locales/ru.mts'

// https://vitepress.dev/reference/site-config
export default defineConfig({
  title: "FO4RP", //Название в шапке
  description: "Официальная страница проекта FO4RP\nhttps://github.com/fonline-roleplay/FO4RP",

  base: '/fo4rp/',
  srcDir: "../docs",
  outDir: '../public',
  lastUpdated: true,
  ignoreDeadLinks: true,
  rewrites: {
    'main/ru/:rest*': ':rest*',
    'main/en/:rest*': 'en/:rest*',
  },

//  head: [
//    ['link', { rel: 'icon', href: '/ixray-1.6-stcop/favicon.ico' }] 
//  ],

  locales: {
    root: ruLocale,
    en: enLocale,
  },

  markdown: {
    config: (md) => {
      md.use(lightbox, {})
    },
  },

  themeConfig: {
//	 logo: '/logo.svg',
    search: {
      provider: 'local',
      options: {}
    },
    socialLinks: [
      { icon: 'github', link: 'https://github.com/fonline-roleplay/FO4RP' },
      { icon: 'discord', link: 'https://discord.gg/6WBYAdjs8D' },
    ]
  }
})