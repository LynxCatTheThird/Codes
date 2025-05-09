// ==UserScript==
// @name         Remove Marketing Background
// @namespace    http://tampermonkey.net/
// @version      0.11
// @description  Hide marketing banners based on different levels
// @match        *://dash.cloudflare.com/*
// @grant        none
// ==/UserScript==

(function() {
    'use strict';

    // 配置不同的隐藏层级 (不包含 "marketing-banner-" 前缀)
    const LEVELS_MAP = {
        'notification-ssl': 0,  // 仅隐藏自身
        'default': 4             // 其他默认隐藏 4 层
    };

    // 向上查找第 n 层父元素的函数
    function getNthParent(element, n) {
        let parent = element;
        for (let i = 0; i < n; i++) {
            if (parent.parentElement) {
                parent = parent.parentElement;
            } else {
                return null; // 如果没有这么多层父元素，返回 null
            }
        }
        return parent;
    }

    // 监听 DOM 变化，自动隐藏符合条件的营销横幅
    const observer = new MutationObserver((mutations) => {
        mutations.forEach(() => {
            const banners = document.querySelectorAll('[data-testid^="marketing-banner-"]');
            banners.forEach((banner) => {
                const testidFull = banner.getAttribute('data-testid'); // 原始的 data-testid
                const testid = testidFull.replace('marketing-banner-', '');  // 去掉前缀

                // 进行前缀匹配，找到最匹配的键
                const matchedKey = Object.keys(LEVELS_MAP).find(key => testid.startsWith(key));
                const LEVELS_UP = matchedKey ? LEVELS_MAP[matchedKey] : LEVELS_MAP.default;

                // 计算要隐藏的目标元素
                const target = LEVELS_UP === 0 ? banner : getNthParent(banner, LEVELS_UP);
                if (target) target.style.display = "none"; // 隐藏目标元素
            });
        });
    });

    // 监听整个页面的 DOM 变化，确保动态内容也能被隐藏
    observer.observe(document.body, { childList: true, subtree: true });
})();
