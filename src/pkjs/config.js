module.exports = [
  {
    type: 'heading',
    defaultValue: 'Watchface Settings',
  },
  {
    type: 'section',
    items: [
      {
        type: 'radiogroup',
        messageKey: 'CATPPUCCIN_FLAVOR',
        label: 'Flavor',
        defaultValue: 'mocha',
        options: [
          { label: 'Latte', value: 'latte' },
          { label: 'Frappe', value: 'frappe' },
          { label: 'Macchiato', value: 'macchiato' },
          { label: 'Mocha', value: 'mocha' },
        ],
      },
    ],
  },
  {
    type: 'submit',
    defaultValue: 'Save Settings',
  },
];
